#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		Part	:Splatter_Tumors.part
		part	:Glow_Tumors.part
		POther  Chest
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow_Tumors.part
		ChildFX	:CHILD_splatter.fx
		POther  Hips
	End


End


#############################################################

End