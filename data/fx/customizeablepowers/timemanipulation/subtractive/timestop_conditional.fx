#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TM_TimeStop_Attack_01 100 100 .8
	End
	Event
		Type	Local
		At	Root
		Sound 	TM_TimeStop_Hit_01 100 100 .8
	End

End

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Hips
		Bhvr	:TimeManip_Shrink_Small.bhvr
		BhvrOverride
			Alpha		150
			FadeInLength	20
			FadeOutLength	20
			TintGeom	1
		End
		Geom	Tintable_TimeManip
		Part	:Swirl_Shrink_Highlights_Big.part
		Part	:Swirl_Stop_Small.part
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	Local
		At	Hips
		Part	:Swirl_Stop_Core_Additive.part
		Part	:Swirl_Stop_Highlights.part
		Part	:Swirl_Stop_Highlights_Big.part
	End
End

Condition
	On	Time
	Time	9

	Event
		Type	Local
		At	Hips
		Part	:Particles_Stop.part
	End
End

#############################################################

End