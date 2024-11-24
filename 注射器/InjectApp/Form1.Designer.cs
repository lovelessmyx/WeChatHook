namespace InjectApp
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            btn_dll_inject = new Button();
            textBox1 = new TextBox();
            label1 = new Label();
            btn_choice_dll = new Button();
            label2 = new Label();
            text_process_id = new TextBox();
            btn_dll_unload = new Button();
            text_process_name = new TextBox();
            label3 = new Label();
            btn_process = new Button();
            label4 = new Label();
            lab_address = new Label();
            button1 = new Button();
            SuspendLayout();
            // 
            // btn_dll_inject
            // 
            btn_dll_inject.Enabled = false;
            btn_dll_inject.Location = new Point(247, 160);
            btn_dll_inject.Name = "btn_dll_inject";
            btn_dll_inject.Size = new Size(94, 29);
            btn_dll_inject.TabIndex = 0;
            btn_dll_inject.Text = "注入DLL";
            btn_dll_inject.UseVisualStyleBackColor = true;
            btn_dll_inject.Click += btn_dll_inject_Click;
            // 
            // textBox1
            // 
            textBox1.Location = new Point(81, 21);
            textBox1.Name = "textBox1";
            textBox1.ReadOnly = true;
            textBox1.Size = new Size(294, 27);
            textBox1.TabIndex = 1;
            textBox1.Text = "O-o";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(9, 24);
            label1.Name = "label1";
            label1.Size = new Size(66, 20);
            label1.TabIndex = 2;
            label1.Text = "DLL地址";
            // 
            // btn_choice_dll
            // 
            btn_choice_dll.Location = new Point(99, 160);
            btn_choice_dll.Name = "btn_choice_dll";
            btn_choice_dll.Size = new Size(94, 29);
            btn_choice_dll.TabIndex = 3;
            btn_choice_dll.Text = "选择DLL";
            btn_choice_dll.UseVisualStyleBackColor = true;
            btn_choice_dll.Click += button1_Click;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(14, 68);
            label2.Name = "label2";
            label2.Size = new Size(54, 20);
            label2.TabIndex = 4;
            label2.Text = "进程ID";
            // 
            // text_process_id
            // 
            text_process_id.Location = new Point(81, 66);
            text_process_id.Name = "text_process_id";
            text_process_id.ReadOnly = true;
            text_process_id.Size = new Size(294, 27);
            text_process_id.TabIndex = 5;
            text_process_id.Text = "o-O";
            // 
            // btn_dll_unload
            // 
            btn_dll_unload.Enabled = false;
            btn_dll_unload.Location = new Point(247, 203);
            btn_dll_unload.Name = "btn_dll_unload";
            btn_dll_unload.Size = new Size(94, 29);
            btn_dll_unload.TabIndex = 6;
            btn_dll_unload.Text = "卸载DLL";
            btn_dll_unload.UseVisualStyleBackColor = true;
            btn_dll_unload.Click += btn_dll_unload_Click;
            // 
            // text_process_name
            // 
            text_process_name.Location = new Point(81, 113);
            text_process_name.Name = "text_process_name";
            text_process_name.ReadOnly = true;
            text_process_name.Size = new Size(294, 27);
            text_process_name.TabIndex = 8;
            text_process_name.Text = "^-^";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(14, 115);
            label3.Name = "label3";
            label3.Size = new Size(54, 20);
            label3.TabIndex = 7;
            label3.Text = "进程名";
            // 
            // btn_process
            // 
            btn_process.Location = new Point(99, 203);
            btn_process.Name = "btn_process";
            btn_process.Size = new Size(94, 29);
            btn_process.TabIndex = 9;
            btn_process.Text = "选择进程";
            btn_process.UseVisualStyleBackColor = true;
            btn_process.Click += btn_process_Click;
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(15, 160);
            label4.Name = "label4";
            label4.Size = new Size(84, 20);
            label4.TabIndex = 10;
            label4.Text = "注入地址：";
            // 
            // lab_address
            // 
            lab_address.AutoSize = true;
            lab_address.Location = new Point(15, 203);
            lab_address.Name = "lab_address";
            lab_address.Size = new Size(0, 20);
            lab_address.TabIndex = 11;
            // 
            // button1
            // 
            button1.Location = new Point(99, 247);
            button1.Name = "button1";
            button1.Size = new Size(94, 30);
            button1.TabIndex = 12;
            button1.Text = "读共享内存";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click_1;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(9F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(406, 289);
            Controls.Add(button1);
            Controls.Add(lab_address);
            Controls.Add(label4);
            Controls.Add(btn_process);
            Controls.Add(text_process_name);
            Controls.Add(label3);
            Controls.Add(btn_dll_unload);
            Controls.Add(text_process_id);
            Controls.Add(label2);
            Controls.Add(btn_choice_dll);
            Controls.Add(label1);
            Controls.Add(textBox1);
            Controls.Add(btn_dll_inject);
            Name = "Form1";
            Text = "注射器";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button btn_dll_inject;
        private TextBox textBox1;
        private Label label1;
        private Button btn_choice_dll;
        private Label label2;
        private TextBox text_process_id;
        private Button btn_dll_unload;
        private TextBox text_process_name;
        private Label label3;
        private Button btn_process;
        private Label label4;
        private Label lab_address;
        private Button button1;
    }
}
