#############################################################
## KineticMelee_Hit.fx
#############################################################

FxInfo
Lifespan 40

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ShockwaveAnchor
		Type	Local
		At	Chest
		SoundNoRepeat 2
		Sound KM_RepulsingTorrent_Hit_01 200 50 1
		Sound KM_RepulsingTorrent_Hit_02 200 50 1
		Sound KM_RepulsingTorrent_Hit_03 200 50 1
		Sound KM_RepulsingTorrent_Hit_04 200 50 1
		Sound KM_RepulsingTorrent_Hit_05 200 50 1
		Part	:Rings_Shockwave_Small.part
		Part	:Rings_Shockwave_Smokey.part
		Part	:Flash_Streaks.part
		Lifespan	5
	End

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate	60 30 30
			PyrRotateJitter 10 0 10
		End
		Part	:Rings_Burst_LocalFacing.part
		Part	:Flash_Core.part
		Part	:Flash_Glow.part
		Sound PP11 200 100 0.7
		Lifespan 	15
		LifespanJitter	5
	End

	Event
		Type	PositOnly
		At	Chest
		BhvrOverride
			PyrRotate	100 0 -30
			PyrRotateJitter 10 0 10

		End
		Part	:Rings_Burst_LocalFacing.part
		Lifespan 	15
		LifespanJitter	5
	End

	Event
		Type	PositOnly
		At	Chest
		BhvrOverride
			PyrRotate	0 120 0
			PyrRotateJitter 20 0 20

		End
		Part	:Rings_Burst_LocalFacing.part
		Lifespan 	15
		LifespanJitter	5
	End


	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Rings_Burst_Smokey.part
		Lifespan 	30
	End

	Event
		Type	PositOnly
		At	Chest
		Part	:Rings_Burst_Rapid.part
		Lifespan 	20
	End
End

Condition
	On	Cycle
	Time	2

	Event
		Type	Local
		At	ShockwaveAnchor
		BhvrOverride
			PyrRotateJitter 180 180 180
		End
		Part	:Rings_Burst_LocalFacing_Rapid.part
		Lifespan 	5
	End
End

#############################################################

End