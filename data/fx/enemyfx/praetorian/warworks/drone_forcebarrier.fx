#############################################################
## Drone_Idle.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Start
		At	Hips
		Bhvr 	behaviors/powers/forcefield/ForceBubble.bhvr
		BhvrOverride
			Alpha		255
			StartColor	0 200 255
			PyrRotateJitter	0.0 0.0 20.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		0 -1.0 0
			SpinJitter	0 .025 0
			EndScale	1.3 1.3 1.3

		End
		Geom	Tintable_EarthGraspHit
	End

	Event
		Type	Start
		At	Hips
		Bhvr 	behaviors/powers/forcefield/ForceBubble.bhvr
		BhvrOverride
			Alpha		255
			StartColor	0 100 255
			PyrRotateJitter	0.0 0.0 20.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		0 1.0 0
			SpinJitter	0 .025 0
			EndScale	1.3 1.3 1.3

		End
		Geom	Tintable_EarthGraspHit
	End

End

#############################################################

End