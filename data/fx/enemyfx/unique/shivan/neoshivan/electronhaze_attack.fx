#############################################################
## ElectronHaze.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time	30
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound 	ShivanNeo_ElectronHaze_Attack_01 200 200 .8
		Sound 	ShivanNeo_ElectronHaze_Attack_02 200 200 .8
		Sound 	ShivanNeo_ElectronHaze_Attack_03 200 200 .8
		Sound 	ShivanNeo_ElectronHaze_Attack_04 200 200 .8
		Sound 	ShivanNeo_ElectronHaze_Attack_05 200 200 .8
		Sound 	ShivanNeo_ElectronHaze_Attack_06 200 200 .8
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	HandAnchorR
		Type	PositOnly
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Hands_Core.part
		Part	:Hands_Rays_Heavy.part
		Part	:Hands_Rings_Heavy.part
		Lifespan 90
	End

	Event
		EName	HandAnchorL
		Type	PositOnly
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Hands_Core.part
		Part	:Hands_Rays_Heavy.part
		Part	:Hands_Rings_Heavy.part
		Lifespan 90
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

Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime
		Type	Start
		At	Root
		Bhvr	behaviors\MedianProjectile1.bhvr
		Magnet	T_Root
		LookAt	T_Root
		Part	:ElectronHaze_Smoke_Alpha.part
		Part	:ElectronHaze_Smoke.part
		Part	:ElectronHaze_Rays.part
		Part	:ElectronHaze_Boils.part

		Sound electronhaze 60 60 0.92
	End
End


#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End