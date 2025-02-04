//------------------------------------------------------------------------------------------------
[ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
class GME_Modules_EditablePointComponentClass : GME_Modules_EditableModuleComponentClass
{
	protected SCR_Faction m_pCallsignFaction;
	protected ref array<int> m_aCallsignIdcs = {};
	
	//------------------------------------------------------------------------------------------------
	LocalizedString GetNextCallsign()
	{
		if (m_aCallsignIdcs.IsEmpty())
		{
			FactionManager factionManager = GetGame().GetFactionManager();
			m_pCallsignFaction = SCR_Faction.Cast(factionManager.GetFactionByIndex(0));
			if (!m_pCallsignFaction)
				return string.Empty;
			
			m_aCallsignIdcs = m_pCallsignFaction.GetBaseCallsignIndexes();
			SCR_ArrayHelperT<int>.Shuffle(m_aCallsignIdcs);
		}
		
		if (m_aCallsignIdcs.IsEmpty())
			return string.Empty;
		
		int idx = m_aCallsignIdcs[0];
		m_aCallsignIdcs.Remove(0);
				
		SCR_MilitaryBaseCallsign callsign = m_pCallsignFaction.GetBaseCallsignByIndex(idx);
		if (callsign)
			return callsign.GetCallsignShort();
		else
			return string.Empty;
	}
}

//------------------------------------------------------------------------------------------------
class GME_Modules_EditablePointComponent  : GME_Modules_EditableModuleComponent
{
	[RplProp(onRplName: "OnCallsignChanged")]
	protected LocalizedString m_sCallsign;
	
	protected ref GME_Modules_EditablePointUIInfo m_CustomUIInfo;
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		GME_Modules_EditablePointComponentClass prefabData = GME_Modules_EditablePointComponentClass.Cast(GetEditableEntityData());
		if (!prefabData)
			return;
		
		m_sCallsign = prefabData.GetNextCallsign();
		OnCallsignChanged();
	}
	
	//------------------------------------------------------------------------------------------------
	void SetCallsign(LocalizedString callsign)
	{
		m_sCallsign = callsign;
		OnCallsignChanged();
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	LocalizedString GetCallsign()
	{
		return m_sCallsign;
	}
	
	//------------------------------------------------------------------------------------------------
	void OnCallsignChanged()
	{
		if (!m_CustomUIInfo)
		{
			m_CustomUIInfo = new GME_Modules_EditablePointUIInfo();
			m_CustomUIInfo.CopyFrom(GetInfo());
			SetInfoInstance(m_CustomUIInfo);
		}
		
		m_CustomUIInfo.SetCallsign(m_sCallsign);
		Event_OnUIRefresh.Invoke();
	}
}
