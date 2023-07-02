class ODIN_TeleportToSquadmateDialog: ChimeraMenuBase
{
	protected static const string TEXT_TITLE = "TextTitle";
	protected static const string BUTTON_CLOSE = "ButtonCancel";
	protected static const string BUTTON_OK = "ButtonOK";
	
	protected SCR_NavigationButtonComponent m_Cancel;
	protected SCR_NavigationButtonComponent m_Confirm;
	
	ref ScriptInvoker m_OnConfirm = new ScriptInvoker();
	ref ScriptInvoker m_OnCancel = new ScriptInvoker();
	
	protected float m_fAnimationRate = UIConstants.FADE_RATE_FAST;
	
	protected OverlayWidget m_ListBoxOverlay;
    protected SCR_ListBoxComponent m_ListBoxComponent;
	
	protected TextWidget m_wTitle;
	
	// passed vars
	protected SCR_EditableEntityComponent m_HoveredEntity;

	//------------------------------------------------------------------------------------------------
	protected override void OnMenuOpen()
	{
		Print("OnMenuOpen: dialog opened!", LogLevel.NORMAL);

		Widget rootWidget = GetRootWidget();
		if (!rootWidget)
		{
			Print("Error in Layout Tutorial layout creation", LogLevel.ERROR);
			return;
		}

		// Texts
		m_wTitle = TextWidget.Cast(rootWidget.FindAnyWidget("TextTitel"));
		
		// Cancel button
		Widget m_wCancelButton = rootWidget.FindAnyWidget("ButtonCancel");
		m_Cancel = SCR_NavigationButtonComponent.GetNavigationButtonComponent("ButtonCancel", rootWidget);
		if (m_Cancel)
			m_Cancel.m_OnActivated.Insert(OnCancel);

		// Confirm button
		m_Confirm = SCR_NavigationButtonComponent.GetNavigationButtonComponent("ButtonOK", rootWidget);
		if (m_Confirm)
			m_Confirm.m_OnActivated.Insert(OnConfirm);
		
		// In order to get the ListBox Index you can use this: m_ListBoxComponent.GetSelectedItem();
		m_ListBoxOverlay = OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBox0"));
        m_ListBoxComponent = SCR_ListBoxComponent.Cast(m_ListBoxOverlay.FindHandler(SCR_ListBoxComponent));
		
		// Play animation
		rootWidget.SetOpacity(0);
		AnimateWidget.Opacity(rootWidget, 1, m_fAnimationRate);
		
		/*
			ESC/Start listener
		*/

		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
		{
			// this is for the menu/dialog to close when pressing ESC
			// an alternative is to have a button with the SCR_NavigationButtonComponent component
			// and its Action Name field set to MenuBack - this would activate the button on ESC press
			inputManager.AddActionListener("MenuOpen", EActionTrigger.DOWN, CloseAnimated);
			inputManager.AddActionListener("MenuBack", EActionTrigger.DOWN, CloseAnimated);
#ifdef WORKBENCH // in Workbench, F10 is used because ESC closes the preview
			inputManager.AddActionListener("MenuOpenWB", EActionTrigger.DOWN, CloseAnimated);
			inputManager.AddActionListener("MenuBackWB", EActionTrigger.DOWN, CloseAnimated);
#endif
		}
		else if (!m_wCancelButton)
		{
			Print("Auto-closing the menu that has no exit path", LogLevel.WARNING);
			Close();
			return;
		}
	}

	//------------------------------------------------------------------------------------------------
	protected override void OnMenuClose()
	{
		// here we clean action listeners added above as the good practice wants it
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
		{
			inputManager.RemoveActionListener("MenuOpen", EActionTrigger.DOWN, CloseAnimated);
			inputManager.RemoveActionListener("MenuBack", EActionTrigger.DOWN, CloseAnimated);
#ifdef WORKBENCH
			inputManager.RemoveActionListener("MenuOpenWB", EActionTrigger.DOWN, CloseAnimated);
			inputManager.RemoveActionListener("MenuBackWB", EActionTrigger.DOWN, CloseAnimated);
#endif
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void AddItemToListbox(string name, Managed data = null)
	{
        if (m_ListBoxComponent)
			m_ListBoxComponent.AddItem(name, data);	
	}
	
	void SetTitle(string title)
	{
		if (m_wTitle)
			m_wTitle.SetText(title);
	}
	
	int getSelectedListBox()
	{
		// null check
		if (!m_ListBoxComponent)
			return -1;	
		
		return m_ListBoxComponent.GetSelectedItem();
	}
	
	Managed getDataForListboxItem(int itemID)
	{
		if (!m_ListBoxComponent)
			return null;
		
		return m_ListBoxComponent.GetItemData(itemID);
	}
	
	void SetHoveredEntity(SCR_EditableEntityComponent owner)
	{
		m_HoveredEntity = owner;
	}
	
	ScriptInvoker GetOnConfirm()
	{
		return m_OnConfirm;
	} 
	
	ScriptInvoker GetOnCancel()
	{
		return m_OnCancel;
	} 
	
	//------------------------------------------------------------------------------------------------
	protected void OnConfirm()
	{
		m_OnConfirm.Invoke(m_HoveredEntity, m_ListBoxComponent);
		CloseAnimated();
	}

	//------------------------------------------------------------------------------------------------
	protected void OnCancel()
	{
		m_OnCancel.Invoke(m_HoveredEntity, m_ListBoxComponent);
		CloseAnimated();
	}
	
	//------------------------------------------------------------------------------------------------
	//! animates dialog closure
	void CloseAnimated()
	{
		AnimateWidget.Opacity(GetRootWidget(), 0, m_fAnimationRate);
		int delay;
		if (m_fAnimationRate > 0)
			delay = 1 / m_fAnimationRate * 1000;
		GetGame().GetCallqueue().CallLater(Close, delay);
	}
}