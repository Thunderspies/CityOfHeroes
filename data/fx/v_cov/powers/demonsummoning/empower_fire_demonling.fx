#############################################################
## Empower_Fire_Demonling.fx
#############################################################

FxInfo
Flags    InheritAlpha

########################################################

Condition
	On 	Time
	Time 	0

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
		Part	:Eyes_Fire_Glow.part
		Part	:Eyes_Fire_Glow_Demonling_Left.part
	End

	Event
		Type	Local
		At	Prime
		 AltPiv 1
		 BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		BhvrOverride
			PositionOffset	-0.135 0.3 0.3
		End
		Part3	:Eyes_Fire_Smoke_Left.part
		Part3	:Eyes_Fire_Fire_Demonling_Left.part
		Part	:Eyes_Fire_Glow_Demonling_Left.part
	End

	Event
		Type	Local
		At	Prime
		AltPiv 2
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		BhvrOverride
			PositionOffset	0.135 0.3 0.3
		End
		Part3	:Eyes_Fire_Smoke.part
		Part3	:Eyes_Fire_Fire_Demonling.part
		Part	:Eyes_Fire_Glow_Demonling.part
	End

End



#########################################################

End
