#############################################################
## Halloween_Sigil_Spawn.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time	0

	Event
		EName	Sigil
		Type	Start
		At	Root
		Bhvr	behaviors\fadein.bhvr
		BhvrOverride
			PositionOffset	 0 0.1 0
			Alpha		255
			StartColor 	128 32 255
			Scale 		60 60 60
		End
		Geom	FX_Sigil_Base
		Sound TorturedWind_Loop 500 500 0.75
	End

	Event
		EName	Rays
		Type	Start
		At	Root
		Bhvr	behaviors\fadein.bhvr
		BhvrOverride
			Alpha		64
			StartColor 	128 32 255
			Scale 		60 60 60
		End
		Geom	FX_SigilRays
	End
End

#########################################################

End