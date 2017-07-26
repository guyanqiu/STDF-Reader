#include "stdf_window.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressBar>
#include <ctime>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    save_action = new QAction(this);
    save_action->setText(tr("Save Values in Table to CSV File"));
    ui->RecordTableWidget->addAction(save_action);
    connect(save_action, SIGNAL(triggered()), this, SLOT(SaveTableToFile()));
    ui->RecordTableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

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

void MainWindow::table_set_value(int row, int col, unsigned int value)
{
    QString str_number;
    str_number.sprintf("%u", value);
    ui->RecordTableWidget->setItem(row, col, new QTableWidgetItem(str_number));
}

void MainWindow::table_set_value(int row, int col, int value)
{
    QString str_number;
    str_number.sprintf("%d", value);
    ui->RecordTableWidget->setItem(row, col, new QTableWidgetItem(str_number));
}

void MainWindow::table_set_value(int row, int col, const char* value)
{
    QString tmp_str = QString::fromLocal8Bit(value);
    ui->RecordTableWidget->setItem(row, col, new QTableWidgetItem(tmp_str));
}

void MainWindow::table_set_value(int row, int col, time_t value)
{
    QString tmp_str = QString::fromLocal8Bit(ctime(&value));
    ui->RecordTableWidget->setItem(row, col, new QTableWidgetItem(tmp_str));
}

void MainWindow::table_set_value(int row, int col, char value)
{
    QString str_number;
    str_number.sprintf("%c", value);
    ui->RecordTableWidget->setItem(row, col, new QTableWidgetItem(str_number));
}

void MainWindow::table_set_value(int row, int col, unsigned char value)
{
    QString str_number;
    str_number.sprintf("%u", value);
    ui->RecordTableWidget->setItem(row, col, new QTableWidgetItem(str_number));
}

void MainWindow::table_set_value(int row, int col, unsigned short value)
{
    QString str_number;
    str_number.sprintf("%u", value);
    ui->RecordTableWidget->setItem(row, col, new QTableWidgetItem(str_number));
}

void MainWindow::table_set_value(int row, int col, float value)
{
    QString str_number;
    str_number.sprintf("%f", value);
    ui->RecordTableWidget->setItem(row, col, new QTableWidgetItem(str_number));
}

void MainWindow::table_set_value(int row, int col, short value)
{
    QString str_number;
    str_number.sprintf("%d", value);
    ui->RecordTableWidget->setItem(row, col, new QTableWidgetItem(str_number));
}

void MainWindow::table_set_value(int row, int col, signed char value)
{
    QString str_number;
    str_number.sprintf("%d", value);
    ui->RecordTableWidget->setItem(row, col, new QTableWidgetItem(str_number));
}

void MainWindow::table_set_value(int row, int col, QString value)
{
    ui->RecordTableWidget->setItem(row, col, new QTableWidgetItem(value));
}

void MainWindow::table_set_flag(int row, int col, unsigned char flag)
{
    QString str_flag;
    str_flag.sprintf("0x%X", flag);
    ui->RecordTableWidget->setItem(row, col, new QTableWidgetItem(str_flag));
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
    dialog->setAcceptMode(QFileDialog::AcceptSave);

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

    table_set_value(0, 0, record->get_cpu_type());     // CPU_TYPE
    table_set_value(0, 1, record->get_stdf_version()); // STDF_VER
	
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

        table_set_value(i, 0, record->get_modify_time());  // SETUP_T
        table_set_value(i, 1, record->get_command_line()); // CMD_LINE

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

        int col = 0;
        table_set_value(i, col++, record->get_setup_time());        // SETUP_T
        table_set_value(i, col++, record->get_start_time());        // START_T
        table_set_value(i, col++, record->get_station_number());    // STAT_NUM
        table_set_value(i, col++, record->get_mode_code());         // MODE_COD
        table_set_value(i, col++, record->get_retest_code());       // RTST_COD
        table_set_value(i, col++, record->get_protection_code());   // PROT_COD
        table_set_value(i, col++, record->get_burn_time());         // BURN_TIM
        table_set_value(i, col++, record->get_command_code());      // CMOD_COD
        table_set_value(i, col++, record->get_lot_id());            // LOT_ID
        table_set_value(i, col++, record->get_part_type());         // PART_TYP
        table_set_value(i, col++, record->get_node_name());         // NODE_NAM
        table_set_value(i, col++, record->get_tester_type());       // TSTR_TYP
        table_set_value(i, col++, record->get_program_name());      // JOB_NAM
        table_set_value(i, col++, record->get_program_revision());  // JOB_REV
        table_set_value(i, col++, record->get_sublot_id());         // SBLOT_ID
        table_set_value(i, col++, record->get_operator_id());       // OPER_NAM
        table_set_value(i, col++, record->get_exec_file_type());    // EXEC_TYP
        table_set_value(i, col++, record->get_exec_file_version()); // EXEC_VER
        table_set_value(i, col++, record->get_test_code());         // TEST_COD
        table_set_value(i, col++, record->get_test_temperature());  // TST_TEMP
        table_set_value(i, col++, record->get_user_text());         // USER_TXT
        table_set_value(i, col++, record->get_auxiliary_filename());// AUX_FILE
        table_set_value(i, col++, record->get_package_type());      // PKG_TYP
        table_set_value(i, col++, record->get_family_id());         // FAMLY_ID
        table_set_value(i, col++, record->get_date_code());         // DATE_COD
        table_set_value(i, col++, record->get_facility_id());       // FACIL_ID
        table_set_value(i, col++, record->get_floor_id());          // FLOOR_ID
        table_set_value(i, col++, record->get_process_id());        // PROC_ID
        table_set_value(i, col++, record->get_operation_freq());    // OPER_FRQ
        table_set_value(i, col++, record->get_spec_name());         // SPEC_NAM
        table_set_value(i, col++, record->get_spec_version());      // SPEC_VER
        table_set_value(i, col++, record->get_testflow_id());       // FLOW_ID
        table_set_value(i, col++, record->get_setup_id());          // SETUP_ID
        table_set_value(i, col++, record->get_design_version());    // DSGN_REV
        table_set_value(i, col++, record->get_engineering_id());    // ENG_ID
        table_set_value(i, col++, record->get_rom_id());            // ROM_COD
        table_set_value(i, col++, record->get_tester_number());     // SERL_NUM
        table_set_value(i, col++, record->get_supervisor_name());   // SUPR_NAM
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

        int col = 0;
        table_set_value(i, col++, record->get_finish_time());      // FINISH_T
        table_set_value(i, col++, record->get_disposition_code()); // DISP_COD
        table_set_value(i, col++, record->get_user_discription()); // USR_DESC
        table_set_value(i, col++, record->get_exec_discription()); // EXC_DESC
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

        int col = 0;
        table_set_value(i, col++, record->get_head_number());    // HEAD_NUM
        table_set_value(i, col++, record->get_site_number());    // SITE_NUM
        table_set_value(i, col++, record->get_part_count());     // PART_CNT
        table_set_value(i, col++, record->get_retest_count());   // RTST_CNT
        table_set_value(i, col++, record->get_abort_count());    // ABRT_CNT
        table_set_value(i, col++, record->get_passed_count());   // GOOD_CNT
        table_set_value(i, col++, record->get_func_test_count());// FUNC_CNT
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
    col_labels<<"HBIN_PF ";
	col_labels<<"HBIN_NAM";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        ui->RecordTableWidget->insertRow(i);
        StdfHBR* record = (StdfHBR*)(stdf_file->get_record(HBR_TYPE, i));

        int col = 0;
        table_set_value(i, col++, record->get_head_number());        //HEAD_NUM
        table_set_value(i, col++, record->get_site_number());        //SITE_NUM
        table_set_value(i, col++, record->get_hardbin_number());     //HBIN_NUM
        table_set_value(i, col++, record->get_hardbin_count());      //HBIN_CNT
        table_set_value(i, col++, record->get_hardbin_indication()); //HBIN_PF
        table_set_value(i, col++, record->get_hardbin_name());       //HBIN_NAM

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

        int col = 0;
        table_set_value(i, col++, record->get_head_number());        // HEAD_NUM
        table_set_value(i, col++, record->get_site_number());        // SITE_NUM
        table_set_value(i, col++, record->get_softbin_number());     // SBIN_NUM
        table_set_value(i, col++, record->get_softbin_count());      // SBIN_CNT
        table_set_value(i, col++, record->get_softbin_indication()); // SBIN_PF
        table_set_value(i, col++, record->get_softbin_name());       // SBIN_NAM

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

        int col = 0;
        table_set_value(i, col++, record->get_pin_index());     // PMR_INDX
        table_set_value(i, col++, record->get_channel_type());  // CHAN_TYP
        table_set_value(i, col++, record->get_channel_name());  // CHAN_NAM
        table_set_value(i, col++, record->get_physical_name()); // PHY_NAM
        table_set_value(i, col++, record->get_logical_name());  // LOG_NAM
        table_set_value(i, col++, record->get_head_number());   // HEAD_NUM
        table_set_value(i, col++, record->get_site_number());   // SITE_NUM
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
	
    col_labels<<"GRP_INDX";
    col_labels<<"GRP_NAM ";
    col_labels<<"INDX_CNT";
    col_labels<<"PMR_INDX";

    for(unsigned int i = 0; i < rec_count; i++)
    {
        StdfPGR* record = (StdfPGR*)(stdf_file->get_record(PGR_TYPE, i));

        int col = 0;
        unsigned short pin_count = record->get_pin_count();
        QString pin_number_list;
        for(unsigned short n = 0; n < pin_count; n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_pin_number(n));
            if(n != pin_count-1) temp += " ,";
            pin_number_list += temp;
        }

        table_set_value(i, col++, record->get_group_index());// GRP_INDX
        table_set_value(i, col++, record->get_group_name()); // GRP_NAM
        table_set_value(i, col++, pin_count);                // INDX_CNT
        table_set_value(i, col++, pin_number_list);          // PMR_INDX
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

        int col = 0;
        unsigned short group_count = record->get_group_count();
        QString group_number_list;
        for(unsigned short n = 0; n < group_count; n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_group_number(n));
            if(n != group_count-1) temp += " ,";
            group_number_list += temp;
        }
        QString group_mode_list;
        for(unsigned short n = 0; n < group_count; n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_group_mode(n));
            if(n != group_count-1) temp += " ,";
            group_mode_list += temp;
        }

        QString group_radix_list;
        for(unsigned short n = 0; n < group_count; n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_group_radix(n));
            if(n != group_count-1) temp += " ,";
            group_radix_list += temp;
        }
        QString program_state_right_list;
        for(unsigned short n = 0; n < group_count; n++)
        {
            QString temp;
            temp = QString::fromLocal8Bit(record->get_program_state_right(n));
            if(n != group_count-1) temp += " ,";
            program_state_right_list += temp;
        }

        QString return_state_right_list;
        for(unsigned short n = 0; n < group_count; n++)
        {
            QString temp;
            temp = QString::fromLocal8Bit(record->get_return_state_right(n));
            if(n != group_count-1) temp += " ,";
            return_state_right_list += temp;
        }

        QString program_state_left_list;
        for(unsigned short n = 0; n < group_count; n++)
        {
            QString temp;
            temp = QString::fromLocal8Bit(record->get_program_state_left(n));
            if(n != group_count-1) temp += " ,";
            program_state_left_list += temp;
        }

        QString return_state_left_list;
        for(unsigned short n = 0; n < group_count; n++)
        {
            QString temp;
            temp = QString::fromLocal8Bit(record->get_return_state_left(n));
            if(n != group_count-1) temp += " ,";
            return_state_left_list += temp;
        }

        table_set_value(i, col++, group_count);              // GRP_CNT
        table_set_value(i, col++, group_number_list);        // GRP_INDX
        table_set_value(i, col++, group_mode_list);          // GRP_MODE
        table_set_value(i, col++, group_radix_list);         // GRP_RADX
        table_set_value(i, col++, program_state_right_list); // PGM_CHAR
        table_set_value(i, col++, return_state_right_list);  // RTN_CHAR
        table_set_value(i, col++, program_state_left_list);  // PGM_CHAL
        table_set_value(i, col++, return_state_left_list);   // RTN_CHAL
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

        int col = 0;
        unsigned short bin_count = record->get_bin_count();
        QString bin_number_list;
        for(unsigned short n = 0; n < bin_count; n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_bin_number(n));
            if(n != bin_count-1) temp += " ,";
            bin_number_list += temp;
        }

        table_set_value(i, col++, bin_count);        // NUM_BINS
        table_set_value(i, col++, bin_number_list);  // RTST_BIN
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

        int col = 0;
        unsigned int site_count = record->get_site_count();
        QString site_number_list;
        for(unsigned int k = 0; k < site_count; k++)
        {
            QString temp;
            temp.sprintf("%u", record->get_site_number(k));
            if(k != site_count-1) temp+= ", ";
            site_number_list += temp;
        }

        table_set_value(i, col++, record->get_head_number());        // HEAD_NUM
        table_set_value(i, col++, record->get_site_group_number());  // SITE_GRP
        table_set_value(i, col++, site_count);                       // SITE_CNT
        table_set_value(i, col++, site_number_list);                 // SITE_NUM
        table_set_value(i, col++, record->get_handler_type());       // HAND_TYP
        table_set_value(i, col++, record->get_handler_id());         // HAND_ID
        table_set_value(i, col++, record->get_probecard_type());     // CARD_TYP
        table_set_value(i, col++, record->get_probecard_id());       // CARD_ID
        table_set_value(i, col++, record->get_loadboard_type());     // LOAD_TYP
        table_set_value(i, col++, record->get_loadboard_id());       // LOAD_ID
        table_set_value(i, col++, record->get_dibboard_type());      // DIB_TYP
        table_set_value(i, col++, record->get_dibboard_id());        // DIB_ID
        table_set_value(i, col++, record->get_cable_type());         // CABL_TYP
        table_set_value(i, col++, record->get_cable_id());           // CABL_ID
        table_set_value(i, col++, record->get_contactor_type());     // CONT_TYP
        table_set_value(i, col++, record->get_contactor_id());       // CONT_ID
        table_set_value(i, col++, record->get_laser_type());         // LASR_TYP
        table_set_value(i, col++, record->get_laser_id());           // LASR_ID
        table_set_value(i, col++, record->get_equipment_type());     // EXTR_TYP
        table_set_value(i, col++, record->get_equipment_id());       // EXTR_ID
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

        int col = 0;
        table_set_value(i, col++, record->get_head_number()); // HEAD_NUM
        table_set_value(i, col++, record->get_group_number());// SITE_GRP
        table_set_value(i, col++, record->get_start_time());  // START_T
        table_set_value(i, col++, record->get_wafer_id());    // WAFER_ID
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

        int col = 0;
        table_set_value(i, col++, record->get_head_number());     // HEAD_NUM
        table_set_value(i, col++, record->get_group_number());    // SITE_GRP
        table_set_value(i, col++, record->get_finish_time());     // FINISH_T
        table_set_value(i, col++, record->get_part_count());      // PART_CNT
        table_set_value(i, col++, record->get_retest_count());    // RTST_CNT
        table_set_value(i, col++, record->get_abort_count());     // ABRT_CNT
        table_set_value(i, col++, record->get_pass_count());      // GOOD_CNT
        table_set_value(i, col++, record->get_func_count());      // FUNC_CNT
        table_set_value(i, col++, record->get_wafer_id());        // WAFER_ID
        table_set_value(i, col++, record->get_fabwafer_id());     // FABWF_ID
        table_set_value(i, col++, record->get_frame_id());        // FRAME_ID
        table_set_value(i, col++, record->get_mask_id());         // MASK_ID
        table_set_value(i, col++, record->get_user_discription());// USR_DESC
        table_set_value(i, col++, record->get_exec_discription());// EXC_DESC
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

        int col = 0;
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

        table_set_value(i, col++, record->get_wafer_size()); // WAFR_SIZ
        table_set_value(i, col++, record->get_die_height()); // DIE_HT
        table_set_value(i, col++, record->get_die_width());  // DIE_WID
        table_set_value(i, col++, wafer_unit);               // WF_UNITS
        table_set_value(i, col++, record->get_wafer_flat()); // WF_FLAT
        table_set_value(i, col++, record->get_center_x());   // CENTER_X
        table_set_value(i, col++, record->get_center_y());   // CENTER_Y
        table_set_value(i, col++, record->get_positive_x()); // POS_X
        table_set_value(i, col++, record->get_positive_y()); // POS_Y
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

        int col = 0;
        table_set_value(i, col++, record->get_head_number()); // HEAD_NUM
        table_set_value(i, col++, record->get_site_number()); // SITE_NUM
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

        int col = 0;
        table_set_value(i, col++, record->get_head_number());           // HEAD_NUM
        table_set_value(i, col++, record->get_site_number());           // SITE_NUM
        table_set_flag(i, col++, record->get_part_information_flag());  // PART_FLG
        table_set_value(i, col++, record->get_number_test());           // NUM_TEST
        table_set_value(i, col++, record->get_hardbin_number());        // HARD_BIN
        table_set_value(i, col++, record->get_softbin_number());        // SOFT_BIN
        table_set_value(i, col++, record->get_x_coordinate());          // X_COORD
        table_set_value(i, col++, record->get_y_coordinate());          // Y_COORD
        table_set_value(i, col++, record->get_elapsed_ms());            // TEST_T(MS)
        table_set_value(i, col++, record->get_part_id());               // PART_ID
        table_set_value(i, col++, record->get_part_discription());      // PART_TXT
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

        int col = 0;
        table_set_value(i, col++, record->get_head_number());        // HEAD_NUM
        table_set_value(i, col++, record->get_site_number());        // SITE_NUM
        table_set_value(i, col++, record->get_test_type());          // TEST_TYP
        table_set_value(i, col++, record->get_test_number());        // TEST_NUM
        table_set_value(i, col++, record->get_exec_count());         // EXEC_CNT
        table_set_value(i, col++, record->get_fail_count());         // FAIL_CNT
        table_set_value(i, col++, record->get_alarm_count());        // ALRM_CNT
        table_set_value(i, col++, record->get_test_name());          // TEST_NAM
        table_set_value(i, col++, record->get_sequencer_name());     // SEQ_NAME
        table_set_value(i, col++, record->get_test_label());         // TEST_LBL
        table_set_flag(i, col++, record->get_optional_data_flag());  // OPT_FLAG
        table_set_value(i, col++, record->get_average_time_s());     // TEST_TIM(S)
        table_set_value(i, col++, record->get_result_min());         // TEST_MIN
        table_set_value(i, col++, record->get_result_max());         // TEST_MAX
        table_set_value(i, col++, record->get_result_sum());         // TST_SUMS
        table_set_value(i, col++, record->get_result_squares_sum()); // TST_SQRS

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

        int col = 0;
        table_set_value(i, col++, record->get_test_number());         // TEST_NUM
        table_set_value(i, col++, record->get_head_number());         // HEAD_NUM
        table_set_value(i, col++, record->get_site_number());         // SITE_NUM
        table_set_flag(i, col++, record->get_test_flag());            // TEST_FLG
        table_set_flag(i, col++, record->get_parametric_test_flag()); // PARM_FLG
        table_set_value(i, col++, record->get_result());              // RESULT
        table_set_value(i, col++, record->get_test_text());           // TEST_TXT
        table_set_value(i, col++, record->get_alarm_id());            // ALARM_ID
        table_set_value(i, col++, record->get_optional_data_flag());  // OPT_FLAG
        table_set_value(i, col++, record->get_result_exponent());     // RES_SCAL
        table_set_value(i, col++, record->get_lowlimit_exponent());   // LLM_SCAL
        table_set_value(i, col++, record->get_highlimit_exponent());  // HLM_SCAL
        table_set_value(i, col++, record->get_low_limit());           // LO_LIMIT
        table_set_value(i, col++, record->get_high_limit());          // HI_LIMIT
        table_set_value(i, col++, record->get_unit());                // UNITS
        table_set_value(i, col++, record->get_result_format());       // C_RESFMT
        table_set_value(i, col++, record->get_lowlimit_format());     // C_LLMFMT
        table_set_value(i, col++, record->get_highlimit_format());    // C_HLMFMT
        table_set_value(i, col++, record->get_low_spec());            // LO_SPEC
        table_set_value(i, col++, record->get_high_spec());           // HI_SPEC

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

        int col = 0;
        unsigned short pin_count = record->get_pin_count();
        QString return_state_list;
        for(unsigned short n = 0; n < pin_count; n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_return_state(n));
            if(n != pin_count-1) temp += ", ";
            return_state_list += temp;
        }
        QString pin_index_list;
        for(unsigned short n = 0; n < pin_count; n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_pin_index(n));
            if(n != pin_count-1) temp += ", ";
            pin_index_list += temp;
        }

        unsigned short result_count = record->get_result_count();
        QString return_result_list;
        for(unsigned short n = 0; n < result_count; n++)
        {
            QString temp;
            temp.sprintf("%f", record->get_return_result(n));
            if(n != result_count-1) temp += ", ";
            return_result_list += temp;
        }

        table_set_value(i, col++, record->get_test_number());        // TEST_NUM
        table_set_value(i, col++, record->get_head_number());        // HEAD_NUM
        table_set_value(i, col++, record->get_site_number());        // SITE_NUM
        table_set_flag(i, col++, record->get_test_flag());           // TEST_FLG
        table_set_flag(i, col++, record->get_parametric_test_flag());// PARM_FLG
        table_set_value(i, col++, pin_count);                        // RTN_ICNT
        table_set_value(i, col++, result_count);                     // RSLT_CNT
        table_set_value(i, col++, return_state_list);                // RTN_STAT
        table_set_value(i, col++, return_result_list);               // RTN_RSLT
        table_set_value(i, col++, record->get_test_text());          // TEST_TXT
        table_set_value(i, col++, record->get_alarm_id());           // ALARM_ID
        table_set_value(i, col++, record->get_optional_data_flag()); // OPT_FLAG
        table_set_value(i, col++, record->get_result_exponent());    // RES_SCAL
        table_set_value(i, col++, record->get_lowlimit_exponent());  // LLM_SCAL
        table_set_value(i, col++, record->get_highlimit_exponent()); // HLM_SCAL
        table_set_value(i, col++, record->get_low_limit());          // LO_LIMIT
        table_set_value(i, col++, record->get_high_limit());         // HI_LIMIT
        table_set_value(i, col++, record->get_starting_input());     // START_IN
        table_set_value(i, col++, record->get_increment_input());    // INCR_IN
        table_set_value(i, col++, pin_index_list);                   // RTN_INDX
        table_set_value(i, col++, record->get_unit());               // UNITS
        table_set_value(i, col++, record->get_unit_input());         // UNITS_IN
        table_set_value(i, col++, record->get_result_format());      // C_RESFMT
        table_set_value(i, col++, record->get_lowlimit_format());    // C_LLMFMT
        table_set_value(i, col++, record->get_highlimit_format());   // C_HLMFMT
        table_set_value(i, col++, record->get_low_spec());           // LO_SPEC
        table_set_value(i, col++, record->get_high_spec());          // HI_SPEC
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

        int col = 0;
        unsigned short pin_count = record->get_pin_count();
        QString pin_number_list;
        for(unsigned short n = 0; n < pin_count; n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_pin_number(n));
            if(n != pin_count-1) temp += ", ";
            pin_number_list += temp;
        }

        QString pin_state_list;
        for(unsigned short n = 0; n < pin_count; n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_pin_state(n));
            if(n != pin_count-1) temp += ", ";
            pin_state_list += temp;
        }

        unsigned short program_state_count = record->get_program_state_count();
        QString program_index_list;
        for(unsigned short n = 0; n < program_state_count; n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_program_index(n));
            if(n != program_state_count-1) temp += ", ";
            program_index_list += temp;
        }

        QString program_state_list;
        for(unsigned short n = 0; n < program_state_count; n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_program_state(n));
            if(n != program_state_count-1) temp += ", ";
            program_state_list += temp;
        }

        QString failpin_data_list;
        for(unsigned short n = 0; n < record->get_failpin_data_count(); n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_failpin_data(n));
            failpin_data_list += temp;
        }

        QString bitmap_data_bits;
        for(unsigned short n = 0; n < record->get_bitmap_data_count(); n++)
        {
            QString temp;
            temp.sprintf("%u", record->get_bitmap_data(n));
            bitmap_data_bits += temp;
        }

        table_set_value(i, col++, record->get_test_number());             // TEST_NUM
        table_set_value(i, col++, record->get_head_number());             // HEAD_NUM
        table_set_value(i, col++, record->get_site_number());             // SITE_NUM
        table_set_flag(i, col++, record->get_test_flag());                // TEST_FLG
        table_set_flag(i, col++, record->get_optional_data_flag());       // OPT_FLAG
        table_set_value(i, col++, record->get_cycle_count());             // CYCL_CNT
        table_set_value(i, col++, record->get_relative_address());        // REL_VADR
        table_set_value(i, col++, record->get_repeat_count());            // REPT_CNT
        table_set_value(i, col++, record->get_failpin_count());           // NUM_FAIL
        table_set_value(i, col++, record->get_xfail_address());           // XFAIL_AD
        table_set_value(i, col++, record->get_yfail_address());           // YFAIL_AD
        table_set_value(i, col++, record->get_vector_offset());           // VECT_OFF
        table_set_value(i, col++, pin_count);                             // RTN_ICNT
        table_set_value(i, col++, program_state_count);                   // PGM_ICNT
        table_set_value(i, col++, pin_number_list);                       // RTN_INDX
        table_set_value(i, col++, pin_state_list);                        // RTN_STAT
        table_set_value(i, col++, program_index_list);                    // PGM_INDX
        table_set_value(i, col++, program_state_list);                    // PGM_STAT
        table_set_value(i, col++, failpin_data_list);                     // FAIL_PIN
        table_set_value(i, col++, record->get_vector_pattern_name());     // VECT_NAM
        table_set_value(i, col++, record->get_timeset_name());            // TIME_SET
        table_set_value(i, col++, record->get_vector_op_code());          // OP_CODE
        table_set_value(i, col++, record->get_test_text());               // TEST_TXT
        table_set_value(i, col++, record->get_alarm_id());                // ALARM_ID
        table_set_value(i, col++, record->get_program_text());            // PROG_TXT
        table_set_value(i, col++, record->get_result_text());             // RSLT_TXT
        table_set_value(i, col++, record->get_pattern_genertor_number()); // PATG_NUM
        table_set_value(i, col++, bitmap_data_bits);                      // SPIN_MAP

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

        int col = 0;
        table_set_value(i, col++, record->get_section_name()); // SEQ_NAME

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
        int col = 0;
        table_set_value(i, col++, QString("EPS"));
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

        int col = 0;
        table_set_value(i, col++, record->get_data_count()); // FLD_CNT
        table_set_value(i, col++, QString("Not Parse."));    // GEN_DATA
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

        int col = 0;
        table_set_value(i, col++, record->get_text_data()); // TEXT_DAT

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
    message += QObject::tr("--2017.06.20 Add Save Tabel to CSV File.\n");
    message += QObject::tr("--2017.07.25 Refactoring the Debug Message Codes.\n");
    message += QObject::tr("--2017.07.26 Refactoring the Record Show Codes.\n");
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
    dialog->setAcceptMode(QFileDialog::AcceptSave);

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
    delete dialog;
}


void MainWindow::SaveTableToFile()
{
    QFileDialog file_dialog(this);
    file_dialog.setWindowTitle(tr("Save Table to CSV File"));
    file_dialog.setNameFilter(tr("CSV Files(*.csv)"));
    file_dialog.setDefaultSuffix("csv");
    file_dialog.setAcceptMode(QFileDialog::AcceptSave);

    if(file_dialog.exec() == QDialog::Accepted)
    {
       QString filename = file_dialog.selectedFiles()[0];
       QFile file(filename);
       bool ret = file.open( QIODevice::Truncate | QIODevice::WriteOnly);
       if(!ret)
       {
           QMessageBox::critical(this, tr("Save File Error"), tr("Save Table To File Failure."),QMessageBox::Ok);
           return;
       }

       QTextStream stream(&file);
       QString conTents;
       QHeaderView * header = ui->RecordTableWidget->horizontalHeader();//ui.m_pTable->horizontalHeader() ;
       if (header)
       {
           for ( int i = 0; i < header->count(); i++ )
           {
               QTableWidgetItem *item = ui->RecordTableWidget->horizontalHeaderItem(i);
               if (!item)
               {
                   conTents +=  ",";
                   continue;
               }
               conTents += item->text() + ",";
           }
           conTents += "\n";
       }

       for ( int i = 0 ; i < ui->RecordTableWidget->rowCount(); i++ )
       {
           for ( int j = 0; j < ui->RecordTableWidget->columnCount(); j++ )
           {

               QTableWidgetItem* item = ui->RecordTableWidget->item(i, j);
               if ( !item )
               {
                   conTents +=  ",";
                   continue;
               }
               QString str = item->text();
               str.replace(","," ");
               conTents += str + ",";
           }
           conTents += "\n";
       }
       stream << conTents;
       file.close();
       QMessageBox::information(this,tr("Save File Success"), tr("Save Table to CSV File Success."),QMessageBox::Ok);
   }
}
