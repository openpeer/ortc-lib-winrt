﻿using DataChannel.Net.Signaling;
using System;
using System.Diagnostics;
using System.Net.NetworkInformation;
using System.Windows.Forms;

namespace DataChannel.Net
{
    public partial class ChatForm : Form
    {
        public Peer p;
        public ChatForm(Peer peer)
        {
            p = peer;
            InitializeComponent();
            Shown += ChatForm_Shown;
        }
        private async void ChatForm_Shown(object sender, EventArgs e)
        {
            var peersListForm = new PeersListForm();
            await peersListForm.InitializeORTC();
            await peersListForm._httpSignaler.SendToPeer(p.Id, "OpenDataChannel");
            await peersListForm.OpenDataChannel(p);
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            Debug.WriteLine("Remote peer id:  " + p.Id + ", Remote peer name:  " + p.Name);

            lstMessages.Items.Add(new Message(DateTime.Now.ToString("h:mm"), GetLocalPeerName() + ":  " + txtMessage.Text));

            //var peersListForm = new PeersListForm();
            //peersListForm._dataChannel.Send(txtMessage.Text);

            txtMessage.Text = string.Empty;
        }

        private string GetLocalPeerName()
        {
            string hostname = IPGlobalProperties.GetIPGlobalProperties().HostName;
            return (hostname != null ? hostname : "<unknown host>") + "-dual";
        }

        
    }
}
