#############################################################
## GlowArms.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## ARMS ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	SpinnerAnchor
		Type	Local
		At	Chest
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.15 0.0
		End
	End

	Event
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
			StartJitter	0.5 0.0 0.0
		End
	End

	Event
		Type	Local
		At	SpinnerAnchor
		part	:Electron_Trail_Stationary_Blue.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	SpinnerAnchor
		Bhvr 	:ForceBubble.bhvr
		BhvrOverride
			Alpha		255
			StartColor	50 100 255
			PyrRotateJitter	0.0 0.0 20.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		0 -.15 0
			SpinJitter	0 .025 0
			EndScale	0.75 0.75 0.75

		End
		Geom	Tintable_EarthGraspHit
		Lifespan	15
	End

	Event
		Type	Local
		At	SpinnerAnchor
		Bhvr 	:ForceBubble.bhvr
		BhvrOverride
			Alpha		255
			StartColor	10 100 255
			PyrRotateJitter	0.0 0.0 20.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		0 -.15 0
			SpinJitter	0 .025 0
			EndScale	0.75 0.75 0.75

		End
		Geom	Tintable_EarthGraspHit
		Lifespan	15
	End

End

#############################################################

End