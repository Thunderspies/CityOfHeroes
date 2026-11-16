#############################################################
## Empower_Hellfire_Demonling.fx
#############################################################

FxInfo
Flags    InheritAlpha

#########################################################
Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0 -.3 0.7
		End
		Part	:Eyes_Hellfire_Glow.part
		Part	:Steam_Hellfire_Small.part
	End

	Event
		Type	Local
		At	Prime
		AltPiv 	1
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		BhvrOverride
			PositionOffset	-0.135 0.3 0.3
		End
		Part	:Eyes_Hellfire_Glow.part
		Part	:Eyes_Hellfire_Fire_Demonling_Left.part
	End

	Event
		Type	Local
		At	Prime
		AltPiv 	2
		BhvrOverride
			PositionOffset	0.135 0.3 0.3
		End
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		Part	:Eyes_Hellfire_Glow.part
		Part	:Eyes_Hellfire_Fire_Demonling.part
	End

End

##################################

End
