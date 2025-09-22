#include "mainwindow.h"
#include "ui_Client_form.h"
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <zmq.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "common/student.h"
#include <QMetaObject>

#ifndef NDEBUG
#include <iostream>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->addressLineEdit->setText("127.0.0.1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_zmqButton_clicked()
{
    ui->zmqButton->setEnabled(false);
    ui->httpButton->setEnabled(false);
    ui->resultTextEdit->setPlainText("Получение данных по ZMQ...");

    std::thread([this](){
        std::string host = ui->addressLineEdit->text().toStdString();
        std::string result_text;
        try {
            #ifndef NDEBUG
            std::cout << "ZMQ: Attempting to connect to tcp://" << host << ":5555" << std::endl;
            #endif
            zmq::context_t context(1);
            zmq::socket_t subscriber(context, zmq::socket_type::sub);
            subscriber.connect("tcp://" + host + ":5555");
            subscriber.set(zmq::sockopt::subscribe, "students");
            subscriber.set(zmq::sockopt::rcvtimeo, 2000); // 2 second timeout

            zmq::message_t topic;
            zmq::message_t content;
            if (subscriber.recv(topic) && subscriber.recv(content)) {
                auto students = nlohmann::json::parse(content.to_string()).get<std::vector<Student>>();
                std::sort(students.begin(), students.end());
                
                std::stringstream ss;
                for(const auto& s : students) {
                    ss << "ID: " << s.id << ", ФИО: " << s.full_name << ", Дата рождения: " << s.birth_date << "\n";
                }
                result_text = ss.str();
            } else {
                result_text = "Ошибка ZMQ: Не удалось получить данные (таймаут).\nУбедитесь, что сервер запущен и адрес указан верно.";
            }
        } catch (const std::exception& e) {
            result_text = "Исключение ZMQ: " + std::string(e.what());
        }
        updateResultText(QString::fromStdString(result_text));
    }).detach();
}

void MainWindow::on_httpButton_clicked()
{
    ui->zmqButton->setEnabled(false);
    ui->httpButton->setEnabled(false);
    ui->resultTextEdit->setPlainText("Получение данных по HTTP...");

    std::thread([this](){
        std::string host = ui->addressLineEdit->text().toStdString();
        std::string result_text;
        try {
            #ifndef NDEBUG
            std::cout << "HTTP: Attempting to connect to http://" << host << ":8080" << std::endl;
            #endif
            httplib::Client cli(host, 8080);
            cli.set_connection_timeout(2); // 2 second timeout
            auto res = cli.Get("/students");

            if (res) {
                if (res->status == 200) {
                    auto students = nlohmann::json::parse(res->body).get<std::vector<Student>>();
                    std::sort(students.begin(), students.end());

                    std::stringstream ss;
                     for(const auto& s : students) {
                        ss << "ID: " << s.id << ", ФИО: " << s.full_name << ", Дата рождения: " << s.birth_date << "\n";
                    }
                    result_text = ss.str();
                } else {
                    result_text = "Ошибка HTTP: Сервер ответил кодом " + std::to_string(res->status);
                }
            } else {
                 auto err = res.error();
                 result_text = "Ошибка подключения по HTTP.\nУбедитесь, что сервер запущен и адрес указан верно.\nКод ошибки: " + httplib::to_string(err);
            }
        } catch (const std::exception& e) {
            result_text = "Исключение HTTP: " + std::string(e.what());
        }
        updateResultText(QString::fromStdString(result_text));
    }).detach();
}

void MainWindow::updateResultText(const QString &text)
{
    QMetaObject::invokeMethod(this, [this, text](){
        ui->resultTextEdit->setPlainText(text);
        ui->zmqButton->setEnabled(true);
        ui->httpButton->setEnabled(true);
    }, Qt::QueuedConnection);
}
