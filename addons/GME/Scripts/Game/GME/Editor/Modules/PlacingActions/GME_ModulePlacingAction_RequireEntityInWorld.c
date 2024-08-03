//------------------------------------------------------------------------------------------------
class GME_ModulePlacingAction_RequireEntityInWorld : GME_ModulePlacingAction_Base
{
	[Attribute(uiwidget: UIWidgets.ResourcePickerThumbnail, desc: "Prefab name of the entity that has to be present in the world", params: "et")]
	protected ResourceName m_sRequiredPrefab;
	
	[Attribute(defvalue: "GME_PLACE_PREFAB_FIRST", uiwidget: UIWidgets.SearchComboBox, desc: "Notification when condition is not met", enums: ParamEnumArray.FromEnum(ENotification))]
	protected ENotification m_eNotificationOnFailure;
	
	//------------------------------------------------------------------------------------------------
	override void OnInitServer()
	{
		m_pModule.RunPlacingActionOwner();
	}
	
	//------------------------------------------------------------------------------------------------
	override void RunOwner(array<IEntity> params = null)
	{
		SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
		set<SCR_EditableEntityComponent> entities = new set<SCR_EditableEntityComponent>();
		core.GetAllEntities(entities);
		
		foreach (SCR_EditableEntityComponent entity : entities)
		{
			if (entity.GetPrefab() == m_sRequiredPrefab)
			{
				m_pModule.OnPlacingActionCompleted();
				return;
			}
		}
		
		SCR_PlacingEditorComponent placingManager = SCR_PlacingEditorComponent.Cast(SCR_PlacingEditorComponent.GetInstance(SCR_PlacingEditorComponent, true, true));
		SCR_NotificationsComponent.SendLocal(m_eNotificationOnFailure, placingManager.GetPrefabID(m_sRequiredPrefab));
		m_pModule.OnPlacingActionCanceled();
	}
}
