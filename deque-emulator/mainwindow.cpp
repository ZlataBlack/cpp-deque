#include "mainwindow.h"

#include "algo.h"
#include "ui_mainwindow.h"

#include <QSignalBlocker>

#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>
#include <string>

namespace
{

const Model::container tea{
    "Чай Лунцзин",
    "Эрл Грей",
    "Сенча",
    "Пуэр",
    "Дарджилинг",
    "Ассам",
    "Матча",
    "Ганпаудер",
    "Оолонг",
    "Лапсанг Сушонг"
};


const Model::container cakes{
    "Красный бархат",
    "Наполеон",
    "Медовик",
    "Тирамису",
    "Прага",
    "Чизкейк",
    "Захер",
    "Эстерхази",
    "Морковный торт",
    "Чёрный лес"
};


bool CaseInsensitiveLess(const std::string& first, const std::string& second){
    const std::size_t size = std::min(first.size(), second.size());

    for (std::size_t i = 0; i < size; ++i){
        const unsigned char first_char = static_cast<unsigned char>(first[i]);

        const unsigned char second_char = static_cast<unsigned char>(second[i]);

        const char first_lower = static_cast<char>(std::tolower(first_char));

        const char second_lower = static_cast<char>(std::tolower(second_char));

        if (first_lower < second_lower){
            return true;
        }

        if (first_lower > second_lower){
            return false;
        }
    }

    return first.size() < second.size();
}

}


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    ApplyModel();
}


MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::SetRandomGen(const std::mt19937& random_gen){
    random_gen_ = random_gen;
}


void MainWindow::ApplyModel(){
    const auto iterator_tmp = deque_model_.iterator;{
        const QSignalBlocker blocker(ui->list_widget);

        ui->list_widget->clear();

        int index = 0;

        for (const std::string& item : deque_model_.items){
            ui->list_widget->addItem(QString::number(index) + ": "
                + QString::fromStdString(item));

            ++index;
        }

        ui->list_widget->addItem("end");
    }

    deque_model_.iterator = iterator_tmp;

    ui->txt_size->setText(QString::number(static_cast<qlonglong>(deque_model_.items.size())));

    const bool has_items =!deque_model_.items.empty();

    ui->btn_erase->setEnabled(has_items);
    ui->btn_pop_front->setEnabled(has_items);
    ui->btn_pop_back->setEnabled(has_items);

    ApplyIterator();
}


void MainWindow::ApplyIterator(){
    const auto begin = deque_model_.items.begin();
    const auto end = deque_model_.items.end();

    if (deque_model_.items.empty()){

        ui->txt_elem_content->clear();
        ui->txt_elem_position->setText("0");

        ui->btn_edit->setEnabled(false);
        ui->btn_erase->setEnabled(false);
        ui->btn_inc_iterator->setEnabled(false);
        ui->btn_dec_iterator->setEnabled(false);

        {
            const QSignalBlocker blocker(ui->list_widget);

            ui->list_widget->setCurrentRow(0);
        }

        return;
    }

    const bool is_begin = deque_model_.iterator == begin;

    const bool is_end = deque_model_.iterator == end;

    ui->btn_dec_iterator->setEnabled(!is_begin);
    ui->btn_inc_iterator->setEnabled(!is_end);

    ui->btn_edit->setEnabled(!is_end);
    ui->btn_erase->setEnabled(!is_end);

    const auto position = std::distance( begin, deque_model_.iterator);

    ui->txt_elem_position->setText(QString::number( static_cast<qlonglong>(position)));

    if (is_end){
        ui->txt_elem_content->clear();

        {
            const QSignalBlocker blocker(ui->list_widget);

            ui->list_widget->setCurrentRow(static_cast<int>(deque_model_.items.size()));
        }

        return;
    }

    ui->txt_elem_content->setText(QString::fromStdString(*deque_model_.iterator));

    {
        const QSignalBlocker blocker(ui->list_widget);

        ui->list_widget->setCurrentRow(static_cast<int>(position));
    }
}


void MainWindow::on_btn_clear_clicked(){
    deque_model_.items.clear();

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_push_front_clicked(){
    const std::string value = ui->txt_elem_content->text().toStdString();

    deque_model_.items.insert(deque_model_.items.begin(), value);

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_push_back_clicked(){
    deque_model_.items.push_back(ui->txt_elem_content->text().toStdString());

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_pop_front_clicked(){
    if (deque_model_.items.empty()){
        return;
    }

    deque_model_.items.erase(deque_model_.items.begin());

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_pop_back_clicked(){
    if (deque_model_.items.empty()){
        return;
    }

    deque_model_.items.pop_back();

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_insert_clicked(){
    const std::string value =
        ui->txt_elem_content->text().toStdString();

    if (deque_model_.iterator == deque_model_.items.end()){
        deque_model_.items.push_back(value);
    }
    else{
        deque_model_.items.insert(deque_model_.iterator,value);
    }

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_erase_clicked(){
    if (deque_model_.items.empty()){
        return;
    }

    if (deque_model_.iterator == deque_model_.items.end()){
        return;
    }

    deque_model_.items.erase(deque_model_.iterator);

    deque_model_.iterator =deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_begin_clicked(){
    deque_model_.iterator = deque_model_.items.begin();

    ApplyIterator();
}


void MainWindow::on_btn_end_clicked(){
    deque_model_.iterator = deque_model_.items.end();

    ApplyIterator();
}


void MainWindow::on_btn_dec_iterator_clicked(){
    if (deque_model_.items.empty()){
        return;
    }

    if (deque_model_.iterator == deque_model_.items.begin()){
        return;
    }

    --deque_model_.iterator;

    ApplyIterator();
}


void MainWindow::on_btn_inc_iterator_clicked(){
    if (deque_model_.items.empty()){
        return;
    }

    if (deque_model_.iterator == deque_model_.items.end()){
        return;
    }

    ++deque_model_.iterator;

    ApplyIterator();
}


void MainWindow::on_list_widget_currentRowChanged(int currentRow){
    currentRow = std::clamp(currentRow,0,static_cast<int>(deque_model_.items.size()));

    deque_model_.iterator = deque_model_.items.begin() + currentRow;

    ApplyIterator();
}


void MainWindow::on_btn_tea_clicked(){
    deque_model_.items = tea;

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_cakes_clicked()
{
    deque_model_.items = cakes;

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_edit_clicked(){
    if (deque_model_.items.empty()){
        return;
    }

    if (deque_model_.iterator == deque_model_.items.end()){
        return;
    }

    *deque_model_.iterator = ui->txt_elem_content->text().toStdString();

    ApplyModel();
}


void MainWindow::on_btn_find_clicked(){
    const std::string value = ui->txt_elem_content->text().toStdString();

    deque_model_.iterator = std::find(deque_model_.items.begin(),
        deque_model_.items.end(), value);

    ApplyIterator();
}


void MainWindow::on_btn_count_clicked(){
    const std::string value = ui->le_count->text().toStdString();

    const auto result =
        std::count(deque_model_.items.begin(),deque_model_.items.end(),value);

    ui->lbl_count->setText(QString::number(static_cast<qlonglong>(result) ));
}


void MainWindow::on_btn_min_element_clicked(){
    if (deque_model_.items.empty()){
        return;
    }

    deque_model_.iterator =
        std::min_element(deque_model_.items.begin(),deque_model_.items.end());

    ApplyIterator();
}


void MainWindow::on_btn_max_element_clicked(){
    if (deque_model_.items.empty()){
        return;
    }

    deque_model_.iterator =
        std::max_element(deque_model_.items.begin(), deque_model_.items.end());

    ApplyIterator();
}


void MainWindow::on_btn_merge_sort_clicked(){
    deque_model_.items =MergeSort( deque_model_.items, std::less<std::string>{});

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_merge_sort_case_insensitive_clicked(){
    deque_model_.items = MergeSort(deque_model_.items,CaseInsensitiveLess);

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_shuffle_clicked(){
    std::shuffle(deque_model_.items.begin(), deque_model_.items.end(),random_gen_);

    ApplyModel();
}


void MainWindow::on_btn_unique_clicked(){
    if (deque_model_.items.empty()){
        return;
    }

    if (!std::is_sorted(deque_model_.items.begin(),deque_model_.items.end())){
        return;
    }

    const auto new_end =std::unique(deque_model_.items.begin(), deque_model_.items.end());

    deque_model_.items.erase(new_end,deque_model_.items.end());

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_reverse_clicked(){
    std::reverse(deque_model_.items.begin(), deque_model_.items.end()
    );

    ApplyModel();
}

void MainWindow::on_btn_resize_clicked(){
    bool ok = false;

    QString text = ui->txt_resize->text().trimmed();

    if (text.isEmpty()){
        text =ui->txt_size->text().trimmed();
    }

    const int new_size =text.toInt(&ok);

    if (!ok || new_size < 0){
        return;
    }

    deque_model_.items.resize(static_cast<std::size_t>(new_size));

    deque_model_.iterator = deque_model_.items.begin();

    ApplyModel();
}


void MainWindow::on_btn_lower_bound_clicked(){
    if (!std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())){
        return;
    }

    const std::string value = ui->txt_elem_content->text().toStdString();

    deque_model_.iterator =std::lower_bound(deque_model_.items.begin(),
            deque_model_.items.end(),value);

    ApplyIterator();
}


void MainWindow::on_btn_upper_bound_clicked(){
    if (!std::is_sorted(deque_model_.items.begin(),deque_model_.items.end())){
        return;
    }

    const std::string value = ui->txt_elem_content->text().toStdString();

    deque_model_.iterator = std::upper_bound(deque_model_.items.begin(),
            deque_model_.items.end(),value);

    ApplyIterator();
}
