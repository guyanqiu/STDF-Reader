#include "stdf_window.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressBar>
#include <ctime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stdf_file = nullptr;
    UpdateUi();
}

MainWindow::~MainWindow()
{
    if(stdf_file)
    {
        delete stdf_file;
        stdf_file = nullptr;
    }
    delete ui;
}

void MainWindow::on_OpenButton_clicked()
{
    QString strTitle = tr("Open STDF File");
    QString strDir = tr("");
    QString strFilter = tr("STDF Files (*.stdf *.std)");

    QFileDialog *fileDialog = new QFileDialog(this, strTitle, strDir, strFilter);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    if(fileDialog->exec() == QDialog::Accepted)
    {
         QStringList file_names_list = fileDialog->selectedFiles();
         filename = file_names_list[0];
         stdf_file = new STDF_FILE();
         int ret = stdf_file->read(filename.toLocal8Bit().data());
         if(ret != 0)
         {
             delete stdf_file;
             stdf_file = nullptr;
             return;
         }

         stdf_types.clear();
         ui->RecordListWidget->clear();
         for(int i = 0; i < STDF_V4_RECORD_COUNT; i++)
         {
             if(stdf_file->get_count((STDF_TYPE)i) > 0)
             {
                 QString rec_name;
				 rec_name = QString::fromLocal8Bit(stdf_file->get_name((STDF_TYPE)i));
                 ui->RecordListWidget->addItem(rec_name);
                 stdf_types.push_back(i);
             }
         }
    }
    if(stdf_types.size() > 0)
    {
        ui->RecordListWidget->setCurrentRow(0);
        ShowRecordTable(STDF_TYPE(stdf_types[0]));
    }
    UpdateUi();
}

void MainWindow::on_ClearButton_clicked()
{
    ui->RecordListWidget->clear();
    ui->RecordTableWidget->clear();
    delete stdf_file;
    stdf_file = nullptr;
    UpdateUi();
}

void MainWindow::on_CloseButton_clicked()
{
    if(stdf_file)
    {
        delete stdf_file;
        stdf_file = nullptr;
    }

	close();
}

void MainWindow::on_SaveButton_clicked()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setWindowTitle("Save TXT File");
    dialog->setNameFilter("TXT Files(*.txt)");
    dialog->setDefaultSuffix("txt");

    if(dialog->exec() == QDialog::Accepted)
    {
        QString savefile = dialog->selectedFiles()[0];
        int ret = stdf_file->write(savefile.toLocal8Bit().data());
        if(ret == 0)
        {
            QMessageBox::information(this,tr("Save File Success"), tr("Save STDF File to TXT File Success."),QMessageBox::Ok);
        }
        else
        {
            QMessageBox::critical(this,tr("Save File Failure"), tr("Save STDF File to TXT File Failure."),QMessageBox::Ok);
        }
    }
}


void MainWindow::UpdateUi()
{
    if(stdf_file)
    {
        ui->OpenButton->hide();
        ui->ClearButton->show();
        ui->SaveButton->show();
        ui->SaveChangeButton->show();
    }
    else
    {
        ui->OpenButton->show();
        ui->ClearButton->hide();
        ui->SaveButton->hide();
        ui->SaveChangeButton->hide();
    }
}



void MainWindow::on_RecordListWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    STDF_TYPE type = (STDF_TYPE)(stdf_types[row]);
    ui->MainStatusBar->showMessage(stdf_file->get_name(type));
    ShowRecordTable(type);

}

void MainWindow::ShowRecordTable(STDF_TYPE type)
{
    ui->RecordTableWidget->clear();
    switch (type)
    {
    case FAR_TYPE: ShowStdfFAR(); break;
    case ATR_TYPE: ShowStdfATR(); break;
	case MIR_TYPE: ShowStdfMIR(); break;
	case MRR_TYPE: ShowStdfMRR(); break;
	case PCR_TYPE: ShowStdfPCR(); break;
	case HBR_TYPE: ShowStdfHBR(); break;
	case SBR_TYPE: ShowStdfSBR(); break;
	case PMR_TYPE: ShowStdfPMR(); break;
	case PGR_TYPE: ShowStdfPGR(); break;
	case PLR_TYPE: ShowStdfPLR(); break;
	case RDR_TYPE: ShowStdfRDR(); break;
	case SDR_TYPE: ShowStdfSDR(); break;
	case WIR_TYPE: ShowStdfWIR(); break;
	case WRR_TYPE: ShowStdfWRR(); break;
	case WCR_TYPE: ShowStdfWCR(); break;
	case PIR_TYPE: ShowStdfPIR(); break;
	case PRR_TYPE: ShowStdfPRR(); break;
	case TSR_TYPE: ShowStdfTSR(); break;
	case PTR_TYPE: ShowStdfPTR(); break;
	case MPR_TYPE: ShowStdfMPR(); break;
	case FTR_TYPE: ShowStdfFTR(); break;
	case BPS_TYPE: ShowStdfBPS(); break;
	case EPS_TYPE: ShowStdfEPS(); break;
	case GDR_TYPE: ShowStdfGDR(); break;
	case DTR_TYPE: ShowStdfDTR(); break;
    default: break;
    }
}

void MainWindow::ShowStdfFAR()
{
	QStringList col_labels;

    unsigned int row_count = stdf_file->get_count(FAR_TYPE);
	unsigned int col_count = 2;
	
    ui->RecordTableWidget->setRowCount(row_count);
	ui->RecordTableWidget->setColumnCount(col_count);

    col_labels<<"CPU_TYPE";
    col_labels<<"STDF_VER";
    
	StdfFAR* record = (StdfFAR*)(stdf_file->get_record(FAR_TYPE, 0));

    QString cpu_type;
    cpu_type.sprintf("%u", record->get_cpu_type());
    ui->RecordTableWidget->setItem(0, 0, new QTableWidgetItem(cpu_type));

    QString stdf_version;
    stdf_version.sprintf("%u", record->get_stdf_version());
    ui->RecordTableWidget->setItem(0, 1, new QTableWidgetItem(stdf_version));
	
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfATR()
{
	QStringList col_labels;

	unsigned int col_count = 2;
	unsigned int rec_count = stdf_file->get_count(ATR_TYPE);
	
    QProgressBar progress_bar(ui->RecordTableWidget);
    progress_bar.setRange(0, rec_count);
    progress_bar.setAlignment(Qt::AlignCenter);
    progress_bar.show();

    ui->RecordTableWidget->setRowCount(0);
    ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"SETUP_T";
    col_labels<<"CMD_LINE";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        ui->RecordTableWidget->insertRow(i);
        StdfATR* record = (StdfATR*)(stdf_file->get_record(ATR_TYPE, i));

        time_t mod_tim = (time_t)(record->get_modify_time());
        QString modify_time;
        modify_time = QString::fromLocal8Bit(ctime(&mod_tim));
        ui->RecordTableWidget->setItem(i, 0, new QTableWidgetItem(modify_time));

        QString cmd_line;
		cmd_line = QString::fromLocal8Bit(record->get_command_line());
        ui->RecordTableWidget->setItem(i, 1, new QTableWidgetItem(cmd_line));
        progress_bar.setValue(i);
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfMIR()
{
	QStringList col_labels;

	unsigned int rec_count = stdf_file->get_count(MIR_TYPE);
	unsigned int col_count = 38;
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"SETUP_T";
    col_labels<<"START_T";
	col_labels<<"STAT_NUM";
	col_labels<<"MODE_COD";
	col_labels<<"RTST_COD";
	col_labels<<"PROT_COD";
	col_labels<<"BURN_TIM";
	col_labels<<"CMOD_COD";
	col_labels<<"LOT_ID  ";
	col_labels<<"PART_TYP";
	col_labels<<"NODE_NAM";
	col_labels<<"TSTR_TYP";
	col_labels<<"JOB_NAM ";
	col_labels<<"JOB_REV ";
	col_labels<<"SBLOT_ID";
	col_labels<<"OPER_NAM";
	col_labels<<"EXEC_TYP";
	col_labels<<"EXEC_VER";
	col_labels<<"TEST_COD";
	col_labels<<"TST_TEMP";
	col_labels<<"USER_TXT";
	col_labels<<"AUX_FILE";
	col_labels<<"PKG_TYP ";
	col_labels<<"FAMLY_ID";
	col_labels<<"DATE_COD";
	col_labels<<"FACIL_ID";
	col_labels<<"FLOOR_ID";
	col_labels<<"PROC_ID ";
	col_labels<<"OPER_FRQ";
	col_labels<<"SPEC_NAM";
	col_labels<<"SPEC_VER";
	col_labels<<"FLOW_ID ";
	col_labels<<"SETUP_ID";
	col_labels<<"DSGN_REV";
	col_labels<<"ENG_ID  ";
	col_labels<<"ROM_COD ";
	col_labels<<"SERL_NUM";
	col_labels<<"SUPR_NAM";
	
    
    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfMIR* record = (StdfMIR*)(stdf_file->get_record(MIR_TYPE, i));

		int row = 0;
        time_t setup_t = (time_t)(record->get_setup_time());
        QString setup_time;
        setup_time = QString::fromLocal8Bit(ctime(&setup_t));
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(setup_time));
		
		
		time_t start_t = (time_t)(record->get_start_time());
        QString start_time;
        start_time = QString::fromLocal8Bit(ctime(&start_t));
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(start_time));
		
		QString station_number;
		station_number.sprintf("%u", record->get_station_number());
		ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(station_number));

        QString mode_code;
        mode_code.sprintf("%c", record->get_mode_code());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(mode_code));
		
		QString retest_code;
        retest_code.sprintf("%c", record->get_retest_code());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(retest_code));
		
		QString protection_code;
        protection_code.sprintf("%c", record->get_protection_code());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(protection_code));
		
		QString burn_time;
        burn_time.sprintf("%u", record->get_burn_time());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(burn_time));
		
		QString command_code;
        command_code.sprintf("%c", record->get_command_code());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(command_code));
		
		QString lot_id;
        lot_id = QString::fromLocal8Bit(record->get_lot_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(lot_id));
		
		QString part_type;
        part_type = QString::fromLocal8Bit(record->get_part_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(part_type));
		
		QString node_name;
        node_name = QString::fromLocal8Bit(record->get_node_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(node_name));
		
		QString tester_type;
        tester_type = QString::fromLocal8Bit(record->get_tester_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(tester_type));
		
		QString program_name;
        program_name = QString::fromLocal8Bit(record->get_program_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(program_name));
		
		QString program_revision;
        program_revision = QString::fromLocal8Bit(record->get_program_revision());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(program_revision));
		
		QString sublot_id;
        sublot_id = QString::fromLocal8Bit(record->get_sublot_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(sublot_id));
		
		QString operator_id;
        operator_id = QString::fromLocal8Bit(record->get_operator_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(operator_id));
		
		QString exec_file_type;
        exec_file_type = QString::fromLocal8Bit(record->get_exec_file_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(exec_file_type));
		
		QString exec_file_version;
        exec_file_version = QString::fromLocal8Bit(record->get_exec_file_version());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(exec_file_version));
		
		QString test_code;
        test_code = QString::fromLocal8Bit(record->get_test_code());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_code));
		
		QString test_temperature;
        test_temperature = QString::fromLocal8Bit(record->get_test_temperature());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_temperature));
		
		QString user_text;
        user_text = QString::fromLocal8Bit(record->get_user_text());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(user_text));
		
		QString auxiliary_filename;
        auxiliary_filename = QString::fromLocal8Bit(record->get_auxiliary_filename());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(auxiliary_filename));
		
		QString package_type;
        package_type = QString::fromLocal8Bit(record->get_package_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(package_type));
		
		QString family_id;
        family_id = QString::fromLocal8Bit(record->get_family_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(family_id));
		
		QString date_code;
        date_code = QString::fromLocal8Bit(record->get_date_code());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(date_code));
		
		QString facility_id;
        facility_id = QString::fromLocal8Bit(record->get_facility_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(facility_id));
		
		QString floor_id;
        floor_id = QString::fromLocal8Bit(record->get_floor_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(floor_id));
        QString process_id;
        process_id = QString::fromLocal8Bit(record->get_process_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(process_id));
		
		QString operation_freq;
        operation_freq = QString::fromLocal8Bit(record->get_operation_freq());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(operation_freq));
		
		QString spec_name;
        spec_name = QString::fromLocal8Bit(record->get_spec_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(spec_name));
		
		QString spec_version;
        spec_version = QString::fromLocal8Bit(record->get_spec_version());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(spec_version));
		
		QString testflow_id;
        testflow_id = QString::fromLocal8Bit(record->get_testflow_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(testflow_id));
		
		QString setup_id;
        setup_id = QString::fromLocal8Bit(record->get_setup_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(setup_id));
		
		QString design_version;
        design_version = QString::fromLocal8Bit(record->get_design_version());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(design_version));
		
		QString engineering_id;
        engineering_id = QString::fromLocal8Bit(record->get_engineering_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(engineering_id));
		
		QString rom_id;
        rom_id = QString::fromLocal8Bit(record->get_rom_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(rom_id));
		
		QString tester_number;
        tester_number = QString::fromLocal8Bit(record->get_tester_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(tester_number));
		
		QString supervisor_name;
        supervisor_name = QString::fromLocal8Bit(record->get_supervisor_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(supervisor_name));
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfMRR()
{
	QStringList col_labels;

	unsigned int col_count = 4;
	unsigned int rec_count = stdf_file->get_count(MRR_TYPE);
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"FINISH_T";
    col_labels<<"DISP_COD";
	col_labels<<"USR_DESC";
	col_labels<<"EXC_DESC";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfMRR* record = (StdfMRR*)(stdf_file->get_record(MRR_TYPE, i));

		int row = 0;
        time_t finish_tim = (time_t)(record->get_finish_time());
        QString finish_time;
        finish_time = QString::fromLocal8Bit(ctime(&finish_tim));
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(finish_time));

        QString disposition_code;
        disposition_code.sprintf("%c", record->get_disposition_code());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(disposition_code));
		
		QString user_discription;
        user_discription = QString::fromLocal8Bit(record->get_user_discription());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(user_discription));
		
		QString exec_discription;
        exec_discription = QString::fromLocal8Bit(record->get_exec_discription());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(exec_discription));
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfPCR()
{
	QStringList col_labels;

	unsigned int col_count = 7;
	unsigned int rec_count = stdf_file->get_count(PCR_TYPE);
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"HEAD_NUM";
    col_labels<<"SITE_NUM";
	col_labels<<"PART_CNT";
	col_labels<<"RTST_CNT";
	col_labels<<"ABRT_CNT";
	col_labels<<"GOOD_CNT";
	col_labels<<"FUNC_CNT";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfPCR* record = (StdfPCR*)(stdf_file->get_record(PCR_TYPE, i));

		int row = 0;
        QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));

        QString site_number;
        site_number.sprintf("%u", record->get_site_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_number));
		
		QString part_count;
        part_count.sprintf("%u", record->get_part_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(part_count));
		
		QString retest_count;
        retest_count.sprintf("%u", record->get_retest_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(retest_count));
		
		QString abort_count;
        abort_count.sprintf("%u", record->get_abort_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(abort_count));
		
		QString passed_count;
        passed_count.sprintf("%u", record->get_passed_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(passed_count));
		
		QString func_test_count;
        func_test_count.sprintf("%u", record->get_func_test_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(func_test_count));
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfHBR()
{
	QStringList col_labels;

	unsigned int col_count = 6;
	unsigned int rec_count = stdf_file->get_count(HBR_TYPE);
	
    QProgressBar progress_bar(ui->RecordTableWidget);
    progress_bar.setRange(0, rec_count);
    progress_bar.setAlignment(Qt::AlignCenter);
    progress_bar.show();

    ui->RecordTableWidget->setRowCount(0);
    ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"HEAD_NUM";
    col_labels<<"SITE_NUM";
	col_labels<<"HBIN_NUM";
	col_labels<<"HBIN_CNT";
	col_labels<<"HBIN_PF";
	col_labels<<"HBIN_NAM";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        ui->RecordTableWidget->insertRow(i);
        StdfHBR* record = (StdfHBR*)(stdf_file->get_record(HBR_TYPE, i));

		int row = 0;
        QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));

        QString site_number;
        site_number.sprintf("%u", record->get_site_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_number));
		
		QString hardbin_number;
        hardbin_number.sprintf("%u", record->get_hardbin_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(hardbin_number));
		
        QString hardbin_count;
        hardbin_count.sprintf("%u", record->get_hardbin_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(hardbin_count));
		
		QString hardbin_indication;
        hardbin_indication.sprintf("%c", record->get_hardbin_indication());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(hardbin_indication));
		
		QString hardbin_name;
        hardbin_name = QString::fromLocal8Bit(record->get_hardbin_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(hardbin_name));
        progress_bar.setValue(i);
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfSBR()
{
	QStringList col_labels;

	unsigned int col_count = 6;
	unsigned int rec_count = stdf_file->get_count(SBR_TYPE);
	
    QProgressBar progress_bar(ui->RecordTableWidget);
    progress_bar.setRange(0, rec_count);
    progress_bar.setAlignment(Qt::AlignCenter);
    progress_bar.show();

    ui->RecordTableWidget->setRowCount(0);
    ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"HEAD_NUM";
    col_labels<<"SITE_NUM";
	col_labels<<"SBIN_NUM";
	col_labels<<"SBIN_CNT";
	col_labels<<"SBIN_PF";
	col_labels<<"SBIN_NAM";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        ui->RecordTableWidget->insertRow(i);
        StdfSBR* record = (StdfSBR*)(stdf_file->get_record(SBR_TYPE, i));

		int row = 0;
        QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));

        QString site_number;
        site_number.sprintf("%u", record->get_site_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_number));
		
		QString softbin_number;
        softbin_number.sprintf("%u", record->get_softbin_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(softbin_number));
		
		QString softbin_count;
        softbin_count.sprintf("%u", record->get_softbin_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(softbin_count));
		
		QString softbin_indication;
        softbin_indication.sprintf("%c", record->get_softbin_indication());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(softbin_indication));
		
		QString softbin_name;
        softbin_name = QString::fromLocal8Bit(record->get_softbin_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(softbin_name));
        progress_bar.setValue(i);
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfPMR()
{
	QStringList col_labels;

	unsigned int col_count = 7;
	unsigned int rec_count = stdf_file->get_count(PMR_TYPE);
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"PMR_INDX";
    col_labels<<"CHAN_TYP";
	col_labels<<"CHAN_NAM";
	col_labels<<"PHY_NAM";
	col_labels<<"LOG_NAM";
	col_labels<<"HEAD_NUM";
	col_labels<<"SITE_NUM";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfPMR* record = (StdfPMR*)(stdf_file->get_record(PMR_TYPE, i));

		int row = 0;
        QString pin_index;
        pin_index.sprintf("%u", record->get_pin_index());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(pin_index));
		
        QString channel_type;
        channel_type.sprintf("%u", record->get_channel_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(channel_type));

		QString channel_name;
        channel_name = QString::fromLocal8Bit(record->get_channel_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(channel_name));
		
		QString physical_name;
        physical_name = QString::fromLocal8Bit(record->get_physical_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(physical_name));
		
		QString logical_name;
        logical_name = QString::fromLocal8Bit(record->get_logical_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(logical_name));
		
		QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));
		
		QString site_number;
        site_number.sprintf("%u", record->get_site_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_number));
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfPGR()
{
	QStringList col_labels;

	unsigned int col_count = 4;
	unsigned int rec_count = stdf_file->get_count(PGR_TYPE);
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"PMR_INDX";
    col_labels<<"CHAN_TYP";
	col_labels<<"CHAN_NAM";
	col_labels<<"PHY_NAM";
	col_labels<<"LOG_NAM";
	col_labels<<"HEAD_NUM";
	col_labels<<"SITE_NUM";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfPGR* record = (StdfPGR*)(stdf_file->get_record(PGR_TYPE, i));

		int row = 0;
        QString group_index;
        group_index.sprintf("%u", record->get_group_index());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(group_index));
		
		QString group_name;
        group_name = QString::fromLocal8Bit(record->get_group_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(group_name));
		
		QString str_pin_count;
		unsigned short pin_count = record->get_pin_count();
        str_pin_count.sprintf("%u", pin_count);
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(str_pin_count));
		
		QString pin_number;
		for(unsigned short n = 0; n < pin_count; n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_pin_number(n));
			if(n != pin_count-1) temp += " ,";
			pin_number += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(pin_number));

    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfPLR()
{
	QStringList col_labels;

	unsigned int col_count = 8;
	unsigned int rec_count = stdf_file->get_count(PLR_TYPE);
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"GRP_CNT";
    col_labels<<"GRP_INDX";
	col_labels<<"GRP_MODE";
	col_labels<<"GRP_RADX";
	col_labels<<"PGM_CHAR";
	col_labels<<"RTN_CHAR";
	col_labels<<"PGM_CHAL";
	col_labels<<"RTN_CHAL";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfPLR* record = (StdfPLR*)(stdf_file->get_record(PLR_TYPE, i));

		int row = 0;
        QString str_group_count;
		unsigned short group_count = record->get_group_count();
        str_group_count.sprintf("%u", group_count);
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(str_group_count));
		
		QString group_number;
        for(unsigned short n = 0; n < group_count; n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_group_number(n));
            if(n != group_count-1) temp += " ,";
			group_number += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(group_number));

		QString group_mode;
        for(unsigned short n = 0; n < group_count; n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_group_mode(n));
            if(n != group_count-1) temp += " ,";
			group_mode += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(group_mode));
		
		QString group_radix;
        for(unsigned short n = 0; n < group_count; n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_group_radix(n));
            if(n != group_count-1) temp += " ,";
			group_radix += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(group_radix));
		
		QString program_state_right;
        for(unsigned short n = 0; n < group_count; n++)
		{
			QString temp;
			temp = QString::fromLocal8Bit(record->get_program_state_right(n));
            if(n != group_count-1) temp += " ,";
			program_state_right += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(program_state_right));
		
		QString return_state_right;
        for(unsigned short n = 0; n < group_count; n++)
		{
			QString temp;
			temp = QString::fromLocal8Bit(record->get_return_state_right(n));
            if(n != group_count-1) temp += " ,";
			return_state_right += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(return_state_right));
		
		QString program_state_left;
        for(unsigned short n = 0; n < group_count; n++)
		{
			QString temp;
			temp = QString::fromLocal8Bit(record->get_program_state_left(n));
            if(n != group_count-1) temp += " ,";
			program_state_left += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(program_state_left));
		
		QString return_state_left;
        for(unsigned short n = 0; n < group_count; n++)
		{
			QString temp;
			temp = QString::fromLocal8Bit(record->get_return_state_left(n));
            if(n != group_count-1) temp += " ,";
			return_state_left += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(return_state_left));
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfRDR()
{
	QStringList col_labels;

	unsigned int col_count = 2;
	unsigned int rec_count = stdf_file->get_count(RDR_TYPE);
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"NUM_BINS";
    col_labels<<"RTST_BIN";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfRDR* record = (StdfRDR*)(stdf_file->get_record(RDR_TYPE, i));

		int row = 0;
        QString str_bin_count;
		unsigned short bin_count = record->get_bin_count();
        str_bin_count.sprintf("%u", bin_count);
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(str_bin_count));

		QString bin_number;
		for(unsigned short n = 0; n < bin_count; n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_bin_number(n));
			if(n != bin_count-1) temp += " ,";
			bin_number += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(bin_number));
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfSDR()
{
	QStringList col_labels;

	unsigned int col_count = 20;
	unsigned int rec_count = stdf_file->get_count(SDR_TYPE);
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"HEAD_NUM";
    col_labels<<"SITE_GRP";
	col_labels<<"SITE_CNT";
	col_labels<<"SITE_NUM";
	col_labels<<"HAND_TYP";
	col_labels<<"HAND_ID ";
	col_labels<<"CARD_TYP";
	col_labels<<"CARD_ID ";
	col_labels<<"LOAD_TYP";
	col_labels<<"LOAD_ID ";
	col_labels<<"DIB_TYP ";
	col_labels<<"DIB_ID  ";
	col_labels<<"CABL_TYP";
	col_labels<<"CABL_ID ";
	col_labels<<"CONT_TYP";
	col_labels<<"CONT_ID ";
	col_labels<<"LASR_TYP";
	col_labels<<"LASR_ID ";
	col_labels<<"EXTR_TYP";
	col_labels<<"EXTR_ID ";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfSDR* record = (StdfSDR*)(stdf_file->get_record(SDR_TYPE, i));

		int row = 0;
        QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));

        QString site_group_number;
        site_group_number.sprintf("%u", record->get_site_group_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_group_number));
		
		QString site_count_str;
		unsigned int site_count = record->get_site_count();
        site_count_str.sprintf("%u", site_count);
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_count_str));
		
		QString site_number;
		for(unsigned int k = 0; k < site_count; k++)
		{
			QString temp;
			temp.sprintf("%u", record->get_site_number(k));
			if(k != site_count-1) temp+= ", ";
			site_number += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_number));
		
		QString handler_type;
        handler_type = QString::fromLocal8Bit(record->get_handler_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(handler_type));
		
		QString handler_id;
        handler_id = QString::fromLocal8Bit(record->get_handler_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(handler_id));
		
		QString probecard_type;
        probecard_type = QString::fromLocal8Bit(record->get_probecard_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(probecard_type));
		
		QString probecard_id;
        probecard_id = QString::fromLocal8Bit(record->get_probecard_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(probecard_id));
		
		QString loadboard_type;
        loadboard_type = QString::fromLocal8Bit(record->get_loadboard_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(loadboard_type));
		
		QString loadboard_id;
        loadboard_id = QString::fromLocal8Bit(record->get_loadboard_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(loadboard_id));
		
		QString dibboard_type;
        dibboard_type = QString::fromLocal8Bit(record->get_dibboard_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(dibboard_type));
		
		QString dibboard_id;
        dibboard_id = QString::fromLocal8Bit(record->get_dibboard_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(dibboard_id));
		
		QString cable_type;
        cable_type = QString::fromLocal8Bit(record->get_cable_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(cable_type));
		
		QString cable_id;
        cable_id = QString::fromLocal8Bit(record->get_cable_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(cable_id));
		
		QString contactor_type;
        contactor_type = QString::fromLocal8Bit(record->get_contactor_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(contactor_type));
		
		QString contactor_id;
        contactor_id = QString::fromLocal8Bit(record->get_contactor_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(contactor_id));
		
		QString laser_type;
        laser_type = QString::fromLocal8Bit(record->get_laser_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(laser_type));
		
		QString laser_id;
        laser_id = QString::fromLocal8Bit(record->get_laser_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(laser_id));
		
		QString equipment_type;
        equipment_type = QString::fromLocal8Bit(record->get_equipment_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(equipment_type));
		
		QString equipment_id;
        equipment_id = QString::fromLocal8Bit(record->get_equipment_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(equipment_id));
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfWIR()
{
	QStringList col_labels;

	unsigned int col_count = 4;
	unsigned int rec_count = stdf_file->get_count(WIR_TYPE);
	
    QProgressBar progress_bar(ui->RecordTableWidget);
    progress_bar.setRange(0, rec_count);
    progress_bar.setAlignment(Qt::AlignCenter);
    progress_bar.show();

    ui->RecordTableWidget->setRowCount(0);
    ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"HEAD_NUM";
    col_labels<<"SITE_GRP";
	col_labels<<"START_T";
	col_labels<<"WAFER_ID";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        ui->RecordTableWidget->insertRow(i);
        StdfWIR* record = (StdfWIR*)(stdf_file->get_record(WIR_TYPE, i));

		int row = 0;
        QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));

		QString group_number;
        group_number.sprintf("%u", record->get_group_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(group_number));
		
		time_t start_t = (time_t)(record->get_start_time());
        QString start_time;
        start_time = QString::fromLocal8Bit(ctime(&start_t));
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(start_time));
		
		QString wafer_id;
        wafer_id = QString::fromLocal8Bit(record->get_wafer_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(wafer_id));
        progress_bar.setValue(i);
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfWRR()
{
	QStringList col_labels;

	unsigned int col_count = 14;
	unsigned int rec_count = stdf_file->get_count(WRR_TYPE);
	
    QProgressBar progress_bar(ui->RecordTableWidget);
    progress_bar.setRange(0, rec_count);
    progress_bar.setAlignment(Qt::AlignCenter);
    progress_bar.show();

    ui->RecordTableWidget->setRowCount(0);
    ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"HEAD_NUM";
    col_labels<<"SITE_GRP";
	col_labels<<"FINISH_T";
	col_labels<<"PART_CNT";
	col_labels<<"RTST_CNT";
	col_labels<<"ABRT_CNT";
	col_labels<<"GOOD_CNT";
	col_labels<<"FUNC_CNT";
    col_labels<<"WAFER_ID";
	col_labels<<"FABWF_ID";
	col_labels<<"FRAME_ID";
	col_labels<<"MASK_ID";
	col_labels<<"USR_DESC";
	col_labels<<"EXC_DESC";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        ui->RecordTableWidget->insertRow(i);
        StdfWRR* record = (StdfWRR*)(stdf_file->get_record(WRR_TYPE, i));

		int row = 0;
        QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));
		
		QString group_number;
        group_number.sprintf("%u", record->get_group_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(group_number));

		time_t finish_t = (time_t)(record->get_finish_time());
        QString finish_time;
        finish_time = QString::fromLocal8Bit(ctime(&finish_t));
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(finish_time));
		
		QString part_count;
        part_count.sprintf("%u", record->get_part_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(part_count));
		
		QString retest_count;
        retest_count.sprintf("%u", record->get_retest_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(retest_count));
		
		QString abort_count;
        abort_count.sprintf("%u", record->get_abort_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(abort_count));
		
		QString pass_count;
        pass_count.sprintf("%u", record->get_pass_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(pass_count));
		
		QString func_count;
        func_count.sprintf("%u", record->get_func_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(func_count));
		
		QString wafer_id;
        wafer_id = QString::fromLocal8Bit(record->get_wafer_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(wafer_id));
		
		QString fabwafer_id;
        fabwafer_id = QString::fromLocal8Bit(record->get_fabwafer_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(fabwafer_id));
		
		QString frame_id;
        frame_id = QString::fromLocal8Bit(record->get_frame_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(frame_id));
		
		QString mask_id;
        mask_id = QString::fromLocal8Bit(record->get_mask_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(mask_id));
		
		QString user_discription;
        user_discription = QString::fromLocal8Bit(record->get_user_discription());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(user_discription));
		
		QString exec_discription;
        exec_discription = QString::fromLocal8Bit(record->get_exec_discription());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(exec_discription));
        progress_bar.setValue(i);
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfWCR()
{
	QStringList col_labels;

	unsigned int col_count = 9;
	unsigned int rec_count = stdf_file->get_count(WCR_TYPE);
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"WAFR_SIZ";
    col_labels<<"DIE_HT";
	col_labels<<"DIE_WID";
	col_labels<<"WF_UNITS";
	col_labels<<"WF_FLAT";
	col_labels<<"CENTER_X";
	col_labels<<"CENTER_Y";
	col_labels<<"POS_X";
	col_labels<<"POS_Y";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfWCR* record = (StdfWCR*)(stdf_file->get_record(WCR_TYPE, i));

		int row = 0;
        QString wafer_size;
        wafer_size.sprintf("%f", record->get_wafer_size());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(wafer_size));

		QString die_height;
        die_height.sprintf("%f", record->get_die_height());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(die_height));
		
		QString die_width;
        die_width.sprintf("%f", record->get_die_width());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(die_width));
		
		QString wafer_unit;
		unsigned char unit_code = record->get_wafer_unit();
		switch(unit_code)
		{
		case 1: wafer_unit = "Inches"; break;
		case 2: wafer_unit = "Centimeters"; break;
		case 3: wafer_unit = "Millimeters"; break;
		case 4: wafer_unit = "Mils"; break;
		default: wafer_unit = "Unknown"; break;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(wafer_unit));
		
		QString wafer_flat;
        wafer_flat.sprintf("%c", record->get_wafer_flat());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(wafer_flat));
		
		QString center_x;
        center_x.sprintf("%d", record->get_center_x());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(center_x));
		
		QString center_y;
        center_y.sprintf("%d", record->get_center_y());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(center_y));
		
		QString positive_x;
        positive_x.sprintf("%c", record->get_positive_x());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(positive_x));
		
		QString positive_y;
        positive_y.sprintf("%c", record->get_positive_y());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(positive_y));
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfPIR()
{
	QStringList col_labels;

	unsigned int col_count = 2;
	unsigned int rec_count = stdf_file->get_count(PIR_TYPE);
	
    QProgressBar progress_bar(ui->RecordTableWidget);
    progress_bar.setRange(0, rec_count);
    progress_bar.setAlignment(Qt::AlignCenter);
    progress_bar.show();

    ui->RecordTableWidget->setRowCount(0);
    ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"HEAD_NUM";
    col_labels<<"SITE_NUM";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        ui->RecordTableWidget->insertRow(i);
        StdfPIR* record = (StdfPIR*)(stdf_file->get_record(PIR_TYPE, i));

		int row = 0;
        QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));

        QString site_number;
        site_number.sprintf("%u", record->get_site_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_number));
        progress_bar.setValue(i);
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfPRR()
{
	QStringList col_labels;

	unsigned int col_count = 11;
	unsigned int rec_count = stdf_file->get_count(PRR_TYPE);
	
    QProgressBar progress_bar(ui->RecordTableWidget);
    progress_bar.setRange(0, rec_count);
    progress_bar.setAlignment(Qt::AlignCenter);
    progress_bar.show();

    ui->RecordTableWidget->setRowCount(0);
    ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"HEAD_NUM";
    col_labels<<"SITE_NUM";
	col_labels<<"PART_FLG";
    col_labels<<"NUM_TEST";
	col_labels<<"HARD_BIN";
    col_labels<<"SOFT_BIN";
	col_labels<<"X_COORD";
    col_labels<<"Y_COORD";
	col_labels<<"TEST_T(MS)";
    col_labels<<"PART_ID";
	col_labels<<"PART_TXT";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        ui->RecordTableWidget->insertRow(i);
        StdfPRR* record = (StdfPRR*)(stdf_file->get_record(PRR_TYPE, i));

		int row = 0;
        QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));

        QString site_number;
        site_number.sprintf("%u", record->get_site_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_number));
		
		QString part_information_flag;
        part_information_flag.sprintf("0x%X", record->get_part_information_flag());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(part_information_flag));
		
		QString number_test;
        number_test.sprintf("%u", record->get_number_test());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(number_test));
		
		QString hardbin_number;
        hardbin_number.sprintf("%u", record->get_hardbin_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(hardbin_number));
		
		QString softbin_number;
        softbin_number.sprintf("%u", record->get_softbin_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(softbin_number));
		
		QString x_coordinate;
        x_coordinate.sprintf("%d", record->get_x_coordinate());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(x_coordinate));
		
		QString y_coordinate;
        y_coordinate.sprintf("%d", record->get_y_coordinate());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(y_coordinate));
		
		QString elapsed_ms;
        elapsed_ms.sprintf("%u", record->get_elapsed_ms());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(elapsed_ms));
		
		QString part_id;
        part_id = QString::fromLocal8Bit(record->get_part_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(part_id));
		
		QString part_discription;
        part_discription = QString::fromLocal8Bit(record->get_part_discription());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(part_discription));
        progress_bar.setValue(i);
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfTSR()
{
	QStringList col_labels;

	unsigned int col_count = 16;
	unsigned int rec_count = stdf_file->get_count(TSR_TYPE);

    QProgressBar progress_bar(ui->RecordTableWidget);
    progress_bar.setRange(0, rec_count);
    progress_bar.setAlignment(Qt::AlignCenter);
    progress_bar.show();
	
    ui->RecordTableWidget->setRowCount(0);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"HEAD_NUM";
    col_labels<<"SITE_NUM";
	col_labels<<"TEST_TYP";
    col_labels<<"TEST_NUM";
	col_labels<<"EXEC_CNT";
    col_labels<<"FAIL_CNT";
	col_labels<<"ALRM_CNT";
	col_labels<<"TEST_NAM";
    col_labels<<"SEQ_NAME";
	col_labels<<"TEST_LBL";
    col_labels<<"OPT_FLAG";
	col_labels<<"TEST_TIM(S)";
	col_labels<<"TEST_MIN";
	col_labels<<"TEST_MAX";
	col_labels<<"TST_SUMS";
	col_labels<<"TST_SQRS";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        ui->RecordTableWidget->insertRow(i);

        StdfTSR* record = (StdfTSR*)(stdf_file->get_record(TSR_TYPE, i));

		int row = 0;
        QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));

        QString site_number;
        site_number.sprintf("%u", record->get_site_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_number));
		
		QString test_type;
        test_type.sprintf("%c", record->get_test_type());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_type));
		
		QString test_number;
        test_number.sprintf("%u", record->get_test_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_number));
		
		QString exec_count;
        exec_count.sprintf("%u", record->get_exec_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(exec_count));
		
		QString fail_count;
        fail_count.sprintf("%u", record->get_fail_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(fail_count));
		
		QString alarm_count;
        alarm_count.sprintf("%u", record->get_alarm_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(alarm_count));
		
		QString test_name;
        test_name = QString::fromLocal8Bit(record->get_test_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_name));
		
		QString sequencer_name;
        sequencer_name = QString::fromLocal8Bit(record->get_sequencer_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(sequencer_name));
		
		QString test_label;
        test_label = QString::fromLocal8Bit(record->get_test_label());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_label));
		
		QString optional_data_flag;
        optional_data_flag.sprintf("0x%X", record->get_optional_data_flag());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(optional_data_flag));
		
		QString average_time_s;
        average_time_s.sprintf("%f", record->get_average_time_s());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(average_time_s));
		
		QString result_min;
        result_min.sprintf("%f", record->get_result_min());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(result_min));
		
		QString result_max;
        result_max.sprintf("%f", record->get_result_max());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(result_max));
		
		QString result_sum;
        result_sum.sprintf("%f", record->get_result_sum());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(result_sum));
		
		QString result_squares_sum;
        result_squares_sum.sprintf("%f", record->get_result_squares_sum());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(result_squares_sum));

        progress_bar.setValue(i);
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}
void MainWindow::ShowStdfPTR()
{
	QStringList col_labels;

	unsigned int col_count = 20;
    unsigned int rec_count = stdf_file->get_count(PTR_TYPE);

    QProgressBar progress_bar(ui->RecordTableWidget);
    progress_bar.setRange(0, rec_count);
    progress_bar.setAlignment(Qt::AlignCenter);
    progress_bar.show();
	

	ui->RecordTableWidget->setColumnCount(col_count);
    ui->RecordTableWidget->setRowCount(0);
	
	col_labels<<"TEST_NUM";
	col_labels<<"HEAD_NUM";
    col_labels<<"SITE_NUM";
	col_labels<<"TEST_FLG";
	col_labels<<"PARM_FLG";
    col_labels<<"RESULT";
	col_labels<<"TEST_TXT";
    col_labels<<"ALARM_ID";
	col_labels<<"OPT_FLAG";
	col_labels<<"RES_SCAL";
	col_labels<<"LLM_SCAL";
    col_labels<<"HLM_SCAL";
	col_labels<<"LO_LIMIT";
    col_labels<<"HI_LIMIT";
	col_labels<<"UNITS";
	col_labels<<"C_RESFMT";
	col_labels<<"C_LLMFMT";
	col_labels<<"C_HLMFMT";
	col_labels<<"LO_SPEC";
	col_labels<<"HI_SPEC";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        ui->RecordTableWidget->insertRow(i);
        StdfPTR* record = (StdfPTR*)(stdf_file->get_record(PTR_TYPE, i));

		int row = 0;
		QString test_number;
        test_number.sprintf("%u", record->get_test_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_number));
		
        QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));

        QString site_number;
        site_number.sprintf("%u", record->get_site_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_number));
		
		QString test_flag;
        test_flag.sprintf("0x%X", record->get_test_flag());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_flag));
		
		QString parametric_test_flag;
        parametric_test_flag.sprintf("0x%X", record->get_parametric_test_flag());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(parametric_test_flag));
		
		QString result;
        result.sprintf("%f", record->get_result());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(result));
		
		QString test_text;
        test_text = QString::fromLocal8Bit(record->get_test_text());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_text));
		
		QString alarm_id;
        alarm_id = QString::fromLocal8Bit(record->get_alarm_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(alarm_id));
		
		QString optional_data_flag;
        optional_data_flag.sprintf("0x%X", record->get_optional_data_flag());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(optional_data_flag));
		
		QString result_exponent;
        result_exponent.sprintf("%d", record->get_result_exponent());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(result_exponent));
		
		QString lowlimit_exponent;
        lowlimit_exponent.sprintf("%d", record->get_lowlimit_exponent());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(lowlimit_exponent));
		
		QString highlimit_exponent;
        highlimit_exponent.sprintf("%d", record->get_highlimit_exponent());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(highlimit_exponent));
		
		QString low_limit;
        low_limit.sprintf("%f", record->get_low_limit());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(low_limit));
		
		QString high_limit;
        high_limit.sprintf("%f", record->get_high_limit());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(high_limit));
		
		QString unit;
        unit = QString::fromLocal8Bit(record->get_unit());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(unit));
		
		QString result_format;
        result_format = QString::fromLocal8Bit(record->get_result_format());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(result_format));
		
		QString lowlimit_format;
        lowlimit_format = QString::fromLocal8Bit(record->get_lowlimit_format());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(lowlimit_format));
		
		QString highlimit_format;
        highlimit_format = QString::fromLocal8Bit(record->get_highlimit_format());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(highlimit_format));
		
		QString low_spec;
        low_spec.sprintf("%f", record->get_low_spec());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(low_spec));
		
		QString high_spec;
        high_spec.sprintf("%f", record->get_high_spec());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(high_spec));

        progress_bar.setValue(i);
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfMPR()
{
	QStringList col_labels;

	unsigned int col_count = 27;
	unsigned int rec_count = stdf_file->get_count(MPR_TYPE);
	
    QProgressBar progress_bar(ui->RecordTableWidget);
    progress_bar.setRange(0, rec_count);
    progress_bar.setAlignment(Qt::AlignCenter);
    progress_bar.show();

    ui->RecordTableWidget->setRowCount(0);
    ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"TEST_NUM";
    col_labels<<"HEAD_NUM";
	col_labels<<"SITE_NUM";
	col_labels<<"TEST_FLG";
	col_labels<<"PARM_FLG";
	col_labels<<"RTN_ICNT";
	col_labels<<"RSLT_CNT";
	col_labels<<"RTN_STAT";
    col_labels<<"RTN_RSLT";
	col_labels<<"TEST_TXT";
	col_labels<<"ALARM_ID";
	col_labels<<"OPT_FLAG";
	col_labels<<"RES_SCAL";
	col_labels<<"LLM_SCAL";
	col_labels<<"HLM_SCAL";
    col_labels<<"LO_LIMIT";
	col_labels<<"HI_LIMIT";
	col_labels<<"START_IN";
	col_labels<<"INCR_IN";
	col_labels<<"RTN_INDX";
	col_labels<<"UNITS";
	col_labels<<"UNITS_IN";
    col_labels<<"C_RESFMT";
	col_labels<<"C_LLMFMT";
	col_labels<<"C_HLMFMT";
	col_labels<<"LO_SPEC";
	col_labels<<"HI_SPEC";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        ui->RecordTableWidget->insertRow(i);
        StdfMPR* record = (StdfMPR*)(stdf_file->get_record(MPR_TYPE, i));

		int row = 0;
        QString test_number;
        test_number.sprintf("%u", record->get_test_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_number));

		QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));
		
		QString site_number;
        site_number.sprintf("%u", record->get_site_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_number));
		
		QString test_flag;
        test_flag.sprintf("0x%X", record->get_test_flag());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_flag));
		
		QString parametric_test_flag;
        parametric_test_flag.sprintf("0x%X", record->get_parametric_test_flag());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(parametric_test_flag));
		
		QString str_pin_count;
		unsigned short pin_count = record->get_pin_count();
        str_pin_count.sprintf("%u", pin_count);
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(str_pin_count));
		
		QString str_result_count;
		unsigned short result_count = record->get_result_count();
        str_result_count.sprintf("%u", result_count);
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(str_result_count));
		
		QString return_state;
		for(unsigned short n = 0; n < pin_count; n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_return_state(n));
			if(n != pin_count-1) temp += ", ";
			return_state += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(return_state));
		
		QString return_result;
		for(unsigned short n = 0; n < result_count; n++)
		{
			QString temp;
			temp.sprintf("%f", record->get_return_result(n));
			if(n != result_count-1) temp += ", ";
			return_result += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(return_result));
		
		QString test_text;
        test_text = QString::fromLocal8Bit(record->get_test_text());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_text));
		
		QString alarm_id;
        alarm_id = QString::fromLocal8Bit(record->get_alarm_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(alarm_id));
		
		QString optional_data_flag;
        optional_data_flag.sprintf("0x%X", record->get_optional_data_flag());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(optional_data_flag));
		
		QString result_exponent;
        result_exponent.sprintf("%d", record->get_result_exponent());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(result_exponent));
		
		QString lowlimit_exponent;
        lowlimit_exponent.sprintf("%d", record->get_lowlimit_exponent());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(lowlimit_exponent));
		
		QString highlimit_exponent;
        highlimit_exponent.sprintf("%d", record->get_highlimit_exponent());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(highlimit_exponent));
		
		QString low_limit;
        low_limit.sprintf("%f", record->get_low_limit());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(low_limit));
		
		QString high_limit;
        high_limit.sprintf("%f", record->get_high_limit());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(high_limit));
		
		QString starting_input;
        starting_input.sprintf("%f", record->get_starting_input());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(starting_input));
		
		QString increment_input;
        increment_input.sprintf("%f", record->get_increment_input());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(increment_input));
		
		QString pin_index;
		for(unsigned short n = 0; n < pin_count; n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_pin_index(n));
			if(n != pin_count-1) temp += ", ";
			pin_index += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(pin_index));
		
		QString unit;
        unit = QString::fromLocal8Bit(record->get_unit());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(unit));
		
		QString unit_input;
        unit_input = QString::fromLocal8Bit(record->get_unit_input());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(unit_input));
		
		QString result_format;
        result_format = QString::fromLocal8Bit(record->get_result_format());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(result_format));
		
		QString lowlimit_format;
        lowlimit_format = QString::fromLocal8Bit(record->get_lowlimit_format());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(lowlimit_format));
		
		QString highlimit_format;
        highlimit_format = QString::fromLocal8Bit(record->get_highlimit_format());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(highlimit_format));
		
		QString low_spec;
        low_spec.sprintf("%f", record->get_low_spec());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(low_spec));
		
		QString high_spec;
        high_spec.sprintf("%f", record->get_high_spec());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(high_spec));
        progress_bar.setValue(i);
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfFTR()
{
	QStringList col_labels;

	unsigned int col_count = 28;
	unsigned int rec_count = stdf_file->get_count(FTR_TYPE);
	
    QProgressBar progress_bar(ui->RecordTableWidget);
    progress_bar.setRange(0, rec_count);
    progress_bar.setAlignment(Qt::AlignCenter);
    progress_bar.show();

    ui->RecordTableWidget->setRowCount(0);
    ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"TEST_NUM";
    col_labels<<"HEAD_NUM";
	col_labels<<"SITE_NUM";
	col_labels<<"TEST_FLG";
	col_labels<<"OPT_FLAG";
	col_labels<<"CYCL_CNT";
	col_labels<<"REL_VADR";
	col_labels<<"REPT_CNT";
    col_labels<<"NUM_FAIL";
	col_labels<<"XFAIL_AD";
	col_labels<<"YFAIL_AD";
	col_labels<<"VECT_OFF";
	col_labels<<"RTN_ICNT";
	col_labels<<"PGM_ICNT";
	col_labels<<"RTN_INDX";
    col_labels<<"RTN_STAT";
	col_labels<<"PGM_INDX";
	col_labels<<"PGM_STAT";
	col_labels<<"FAIL_PIN";
	col_labels<<"VECT_NAM";
	col_labels<<"TIME_SET";
	col_labels<<"OP_CODE";
    col_labels<<"TEST_TXT";
	col_labels<<"ALARM_ID";
	col_labels<<"PROG_TXT";
	col_labels<<"RSLT_TXT";
	col_labels<<"PATG_NUM";
	col_labels<<"SPIN_MAP";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        ui->RecordTableWidget->insertRow(i);
        StdfFTR* record = (StdfFTR*)(stdf_file->get_record(FTR_TYPE, i));

		int row = 0;
        QString test_number;
        test_number.sprintf("%u", record->get_test_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_number));
		
		QString head_number;
        head_number.sprintf("%u", record->get_head_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(head_number));
		
		QString site_number;
        site_number.sprintf("%u", record->get_site_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(site_number));

		QString test_flag;
        test_flag.sprintf("0x%X", record->get_test_flag());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_flag));
		
		QString optional_data_flag;
        optional_data_flag.sprintf("0x%X", record->get_optional_data_flag());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(optional_data_flag));
		
		QString cycle_count;
        cycle_count.sprintf("%u", record->get_cycle_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(cycle_count));
		
		QString relative_address;
        relative_address.sprintf("%u", record->get_relative_address());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(relative_address));
		
		QString repeat_count;
        repeat_count.sprintf("%u", record->get_repeat_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(repeat_count));
		
		QString failpin_count;
        failpin_count.sprintf("%u", record->get_failpin_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(failpin_count));
		
		QString xfail_address;
        xfail_address.sprintf("%d", record->get_xfail_address());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(xfail_address));
		
		QString yfail_address;
        yfail_address.sprintf("%d", record->get_yfail_address());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(yfail_address));
		
		QString vector_offset;
        vector_offset.sprintf("%d", record->get_vector_offset());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(vector_offset));
		
		QString str_pin_count;
		unsigned short pin_count = record->get_pin_count();
        str_pin_count.sprintf("%u", pin_count);
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(str_pin_count));
		
		QString str_program_state_count;
		unsigned short program_state_count = record->get_program_state_count();
        str_program_state_count.sprintf("%u", program_state_count);
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(str_program_state_count));
		
		QString pin_number;
		for(unsigned short n = 0; n < pin_count; n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_pin_number(n));
			if(n != pin_count-1) temp += ", ";
			pin_number += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(pin_number));
		
		QString pin_state;
		for(unsigned short n = 0; n < pin_count; n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_pin_state(n));
			if(n != pin_count-1) temp += ", ";
			pin_state += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(pin_state));
		
		QString program_index;
		for(unsigned short n = 0; n < program_state_count; n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_program_index(n));
			if(n != program_state_count-1) temp += ", ";
			program_index += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(program_index));
		
		QString program_state;
		for(unsigned short n = 0; n < program_state_count; n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_program_state(n));
			if(n != program_state_count-1) temp += ", ";
			program_state += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(program_state));
		
		QString failpin_data;
		for(unsigned short n = 0; n < record->get_failpin_data_count(); n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_failpin_data(n));
			failpin_data += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(failpin_data));
		
		QString vector_pattern_name;
        vector_pattern_name = QString::fromLocal8Bit(record->get_vector_pattern_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(vector_pattern_name));
		
		QString timeset_name;
        timeset_name = QString::fromLocal8Bit(record->get_timeset_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(timeset_name));
		
		QString vector_op_code;
        vector_op_code = QString::fromLocal8Bit(record->get_vector_op_code());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(vector_op_code));
		
		QString test_text;
        test_text = QString::fromLocal8Bit(record->get_test_text());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(test_text));
		
		QString alarm_id;
        alarm_id = QString::fromLocal8Bit(record->get_alarm_id());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(alarm_id));
		
		QString result_text;
        result_text = QString::fromLocal8Bit(record->get_result_text());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(result_text));
		
		QString pattern_genertor_number;
        pattern_genertor_number.sprintf("%u", record->get_pattern_genertor_number());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(pattern_genertor_number));
		
		QString bitmap_data;
		for(unsigned short n = 0; n < record->get_bitmap_data_count(); n++)
		{
			QString temp;
			temp.sprintf("%u", record->get_bitmap_data(n));
			bitmap_data += temp;
		}
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(bitmap_data));
        progress_bar.setValue(i);
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfBPS()
{
	QStringList col_labels;

	unsigned int col_count = 1;
	unsigned int rec_count = stdf_file->get_count(BPS_TYPE);
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"SEQ_NAME";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfBPS* record = (StdfBPS*)(stdf_file->get_record(BPS_TYPE, i));

		int row = 0;
        QString section_name;
        section_name = QString::fromLocal8Bit(record->get_section_name());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(section_name));

    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfEPS()
{
	QStringList col_labels;

	unsigned int col_count = 1;
	unsigned int rec_count = stdf_file->get_count(EPS_TYPE);
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"EPS";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        //StdfEPS* record = (StdfEPS*)(stdf_file->get_record(EPS_TYPE, i));

		int row = 0;
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem("EPS"));
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfGDR()
{
	QStringList col_labels;

	unsigned int col_count = 2;
	unsigned int rec_count = stdf_file->get_count(GDR_TYPE);
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"FLD_CNT";
    col_labels<<"GEN_DATA";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfGDR* record = (StdfGDR*)(stdf_file->get_record(GDR_TYPE, i));

		int row = 0;
        QString data_count;
        data_count.sprintf("%u", record->get_data_count());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(data_count));

		ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem("Not Parse."));
    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}

void MainWindow::ShowStdfDTR()
{
	QStringList col_labels;

	unsigned int col_count = 1;
	unsigned int rec_count = stdf_file->get_count(DTR_TYPE);
	
    ui->RecordTableWidget->setRowCount(rec_count);
	ui->RecordTableWidget->setColumnCount(col_count);
	
	col_labels<<"TEXT_DAT";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfDTR* record = (StdfDTR*)(stdf_file->get_record(DTR_TYPE, i));

		int row = 0;
        QString text_data;
        text_data = QString::fromLocal8Bit(record->get_text_data());
        ui->RecordTableWidget->setItem(i, row++, new QTableWidgetItem(text_data));

    }
	ui->RecordTableWidget->setHorizontalHeaderLabels(col_labels);
	ui->RecordTableWidget->resizeColumnsToContents();
}



void MainWindow::on_actionAbout_triggered()
{
    QString title = QObject::tr("Update Information");
    QString message = QObject::tr("This Program Just for the STDF V4 Format Check, and Only Support the CPU_TYP = 2.\n");
    message += QObject::tr("--2016.04.08 Create New Program for STDF File Check.\n");
    message += QObject::tr("--2016.04.14 Refactoring Codes of STDF Part.\n");
    message += QObject::tr("--2016.06.07 Add Save As Function and Fixed STDF File Write Bug.\n");
    message += QObject::tr("--2017.01.12 Fiexd MIR missing PROC_ID bug.\n");
    QMessageBox msgDlg(QMessageBox::Information, title, message, QMessageBox::Ok,NULL);
    msgDlg.exec();
}

void MainWindow::on_actionHelp_triggered()
{
    QString title = QObject::tr("Help Information");
    QString message;
    message += QObject::tr("1.    Open An STDF File\n");
    message += QObject::tr("2.    The Records Show in the ListBox at Left\n");
    message += QObject::tr("3.    Select the Record Type in ListBox\n");
    message += QObject::tr("4.    The Detials Show in the Table at Right\n");
    QMessageBox msgDlg(QMessageBox::Information, title, message, QMessageBox::Ok,NULL);
    msgDlg.exec();
}

void MainWindow::on_SaveChangeButton_clicked()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setWindowTitle("Save STDF File");
    dialog->setNameFilter("STDF Files(*.stdf)");
    dialog->setDefaultSuffix("stdf");

    if(dialog->exec() == QDialog::Accepted)
    {
        QString savefile = dialog->selectedFiles()[0];
        int ret = stdf_file->save(savefile.toLocal8Bit().data());
        if(ret == 0)
        {
            QMessageBox::information(this,tr("Save File Success"), tr("Save STDF File Success."),QMessageBox::Ok);
        }
        else
        {
            QMessageBox::critical(this,tr("Save File Failure"), tr("Save STDF File Failure."),QMessageBox::Ok);
        }
    }
}
