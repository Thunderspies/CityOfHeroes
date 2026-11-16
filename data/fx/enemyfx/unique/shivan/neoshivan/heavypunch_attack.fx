#############################################################
## ChestBeam_Hit.fx
#############################################################

FxInfo
Lifespan 100

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		SoundNoRepeat 1
		Sound 	Shivan_Punch_Attack_01 200 200 .8
		Sound 	Shivan_Punch_Attack_02 200 200 .8
		Sound 	Shivan_Punch_Attack_03 200 200 .8
		Sound 	Shivan_Punch_Attack_04 200 200 .8
	End

	Event
		Type	Local
		At	Root
		SoundNoRepeat 4
		Sound 	Shivan_PunchVOX_01 30 30 .8
		Sound 	Shivan_PunchVOX_02 30 30 .8
		Sound 	Shivan_PunchVOX_03 30 30 .8
		Sound 	Shivan_PunchVOX_04 30 30 .8
		Sound 	Shivan_PunchVOX_05 30 30 .8
		Sound 	Shivan_PunchVOX_06 30 30 .8
	End

	Event
		EName	HandAnchorR
		Type	PositOnly
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Hands_Core.part
		Part	:Hands_Rays_Heavy.part
		Part	:Hands_Rings_Heavy.part
		Lifespan 75
	End

	Event
		EName	HandAnchorR
		Type	PositOnly
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Hands_Core.part
		Part	:Hands_Rays_Heavy.part
		Part	:Hands_Rings_Heavy.part
		Lifespan 75
	End
End


Condition
	On 	Cycle
	Time 	8

	Event
		Type	PositOnly
		At	HandR #SpinnerAnchor
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.35 0.0
		End
		part	:Electron_Trail_Stationary_Hand.part
		Lifespan 15
	End

	Event
		Type	PositOnly
		At	HandL #SpinnerAnchor
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.35 0.0
		End
		part	:Electron_Trail_Stationary_Hand.part
		Lifespan 15
	End

End


#############################################################

End