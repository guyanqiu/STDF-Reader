#ifndef STDF_WINDOW_H
#define STDF_WINDOW_H

#include <QMainWindow>
#include "stdf_v4_file.h"
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_OpenButton_clicked();

    void on_ClearButton_clicked();

    void on_CloseButton_clicked();

    void on_RecordListWidget_clicked(const QModelIndex &index);

    void on_SaveButton_clicked();

    void on_actionAbout_triggered();

    void on_actionHelp_triggered();

    void on_SaveChangeButton_clicked();


private:
    void UpdateUi();
    void ShowRecordTable(STDF_TYPE type);
    void ShowStdfFAR();
	void ShowStdfATR();
	void ShowStdfMIR();
	void ShowStdfMRR();
	void ShowStdfPCR();
	void ShowStdfHBR();
	void ShowStdfSBR();
	void ShowStdfPMR();
	void ShowStdfPGR();
	void ShowStdfPLR();
	void ShowStdfRDR();
	void ShowStdfSDR();
	void ShowStdfWIR();
	void ShowStdfWRR();
	void ShowStdfWCR();
	void ShowStdfPIR();
	void ShowStdfPRR();
	void ShowStdfTSR();
	void ShowStdfPTR();
	void ShowStdfMPR();
	void ShowStdfFTR();
	void ShowStdfBPS();
	void ShowStdfEPS();
	void ShowStdfGDR();
	void ShowStdfDTR();

private:
    Ui::MainWindow *ui;
    STDF_FILE *stdf_file;
    QString filename;
    std::vector<int> stdf_types;
};

#endif // STDF_WINDOW_H
