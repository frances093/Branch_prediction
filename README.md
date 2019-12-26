# Branch prediction
## 簡介：  
* 實作branch prediction，至少要做一種預測器。output為預測結果與misprediction統計數量

    * 基礎要求：2bit counter, input為Taken/Not Taken序列 (predictor的entry數量為無限大)
    * 高分要求：2bit history, input為一個序列的PC與instruction 同時可以設定predictor的entry數量
* 使用語言：C++。


## Basic
### Input：
* 檔案：（要和程式放在同一層）只包含一行由 N 及 T 所組成的字串的 .txt 檔案（名稱：pre_basic.txt）。

        TTN
### Output：  
   * 執行視窗：
            
            State                   pred.   outcome         miss?
            (00,SN,SN,SN,SN)        N       T               miss!
            (01,WN,SN,SN,SN)        N       T               miss!
            (11,WN,WN,SN,SN)        N       N

            Total predict time: 3
            Total miss time: 2
   * 檔案：（名稱：pre_output_basic.txt）
    
            Total predict time: 3
            Total miss time: 2
### Step：  
1.　讀檔，回傳目標字串：

       string readfile() 

2.　依次讀入目標字串的各個字元。  
3.　根據 predictor 目前內容判定預測為 taken 或 not taken ，並和目前讀到的字元做比較，印出本次執行為 taken 或 nottaken 以及是否預測 miss ，並將是否 miss 回傳：

       bool printout(int *mode, int *p, int predict, bool outcome, bool miss) 
    

*  此 function 會呼叫：

 
            void printstate(int history) 
            
    以上 function 會依照目前 predictior 的狀態印出相應的 state （SN、WN、WT、ST）

4.　根據此次從目標字串讀入的字元修改 predictor 狀態。  
*    此時若 history 數值大於 3 或小於 0 ，會呼叫：
    
            int fixhistory(int history) 
        
        用於將 history 恢復成 0 （SN） 或 3 （ST）。  

5.　重複 2. ~ 4. 直到整個字串讀完。  
6.　將統計結果 print 在執行視窗。  
7.　輸出統計結果至檔案。  
8.　結束運行。     
## Inhance
### Input：
* 一個 .txt 檔（名稱：pre_inhance.txt），檔案中第一行為指定的 entry 數量，第二行要開始為要執行的 instructions 。

        14
        0x110    addi R1,R0,0
        0x114    addi R2,R0,0
                 Loop:
        0x11C    beq R1,R2,End
        0x120    subi R2,R2,1
        0x124    beq R0,R0,Loop
	             End:
### Output：  
* 執行視窗：
            
        Total entry: 14

                Predictor       State                   predict         outcome         miss?   R0 R1 R2 R3 R4 R5 R6 R7
        0x110   0               (00,SN,SN,SN,SN)        N               N                       0  0  0  0  0  0  0  0
        0x114   1               (00,SN,SN,SN,SN)        N               N                       0  0  0  0  0  0  0  0
        0x11C   2               (00,SN,SN,SN,SN)        N               T               miss!   0  0  0  0  0  0  0  0
        Predict time: 3
        Miss time: 1
* 檔案 a：（名稱：entryN.txt，檔名中 N 是根據輸出的是哪個 entry 的 predictor 的追蹤結果而定，以下為 entry2.txt 的內容）
    
        	State		    	pred.	outcome	    miss?	
        0x11C	(00,SN,SN,SN,SN)	N	T	    miss!

* 檔案 b：（名稱：pre_outcome_inhance.txt）

        Total entry: 14
        Total predict time: 3
        Total miss time: 1


### Step：
1. 讀取輸入檔案中的 entry 數量及 instruction ：

        void readfile() 
2. 將讀入的 instruction 轉成 int 表示法：

            void transinst()
        
    * 在轉換時會呼叫：

            int getnum(string a)    或    int findnum(string a)
        兩者皆會回傳與輸入 string 相應的 int ，後者專門處理不只一個位數的數字。

3. 生成輸入檔中所指定的 entry 數：

            void generatepre()
4. 根據目前的 pc （一開始設為 0 ）找到目前要使用的 predictor 。  
5. 根據前一步結果 access predictor 取得目前該 predictor 處於哪個 mode ，並印出目前 predictor 的狀態：
            
        int set2bitcount(Predictor *ptrp, int now_pred)
    * 以上function會呼叫：

            string getstate(int history) 

        此 function 會回傳 SN 或 WN 或 WT 或 ST 協助 print 出 predictor 目前的狀態。

6. 根據前兩步結果， access predictor 判定預測為 taken 或 not taken ：

        bool predict(Predictor *ptrp,int now)

7. 根據目前 pc 執行相應 instruction （執行結果包含修改 register 內容及 pc 的數值）並判定此 instruction 是否有 branch taken ：

        bool operate()
    
8. 根據 6. 和 7. 的結果判定此次 prediction 是否 miss ：

        bool is_miss(bool pre, bool out)

9. 印出執行 instrction 後的 registers 目前狀態：

        void registerstate()

10. 將此次的 predict 結果、 outcome 是否 branch 、是否 miss 以及 predictor 狀態寫入相應的 entry.txt 中：

        void outputthisstate(int now_pred, int now_mode, int pred, int outcome, int miss, Predictor *ptrp, int oldpc)
    
    *    此funtion會呼叫：

                char getchar(int a)
            協助取得要寫入的檔案的檔名。
    
11. 根據 7. 的結果修改目前的 predictor ：

        void fixpredictor(bool outcome, int now_mode, Predictor *ptrp)

    *    此 function 可能會呼叫：

                void fixhistory(int *history) 
            防止 history 的數值超過有效範圍（ 0 ~ 3 ）。  
12. 重複 4. ~ 11. 直至 pc 數值超過 instruction 範圍為止。  
13. 將本次執行統計的 predict 次數及 miss 次數寫入檔案：  

		void outputmisscount(int predictime, int miss)
            
14. print 出本次執行統計的 predict 次數及 miss 次數。  
15. 結束運行。
