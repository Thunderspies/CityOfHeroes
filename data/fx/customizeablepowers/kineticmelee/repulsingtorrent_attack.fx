#############################################################
## RepulsingTorrent_Attack.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On Time
	Time 10
	Event
		Type Local
		At Root
		Sound KM_RepulsingTorrent_Attack_01 200 200 .8
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	HandR
		Part	:Hand_Trails.part
		Part	:Hand_Trails_Rings.part
		Lifespan	35
	End

	Event
		Type	Local
		At	HandL
		Part	:Hand_Trails_Rings.part
		Lifespan	45
	End

	Event
		Type	Local
		At	HandL
		Part	:Hand_Trails.part
		Lifespan	40
	End
End

Condition
	On	Time
	Time	15

	Event
		Ename	Spinner
		Type	PositOnly
		At	Chest
		BhvrOverride
			Spin		0.0 0.4 0.2
			SpinJitter 	0.0 0.1 0.0
		End
		Lifespan 20
	End

	Event
		Ename	Spinner2
		Type	PositOnly
		At	Chest
		BhvrOverride
			Spin		0.2 -0.4 0.0
			SpinJitter 	0.0 0.1 0.0
		End
		Lifespan 20
	End
End

Condition
	On	Time
	Time	16
	Repeat	1

	Event
		Type	Local
		At	Spinner
		BhvrOverride
			PositionOffset 	8.0 7.0 0.0
			StartJitter	2.0 4.0	4.5
			Trackrate	0.75
		End
		Part	:GustTrails.part
		LookAt	Chest
		Magnet	Chest
		Lifespan 20
	End


	Event
		Type	Local
		At	Spinner2
		BhvrOverride
			PositionOffset 	-8.0 7.0 0.0
			StartJitter	2.0 4.0 4.5
			Trackrate	0.75
		End
		Part	:GustTrails.part
		LookAt	Chest
		Magnet	Chest
		Lifespan 20
	End
End

Condition
	On	Time
	Time	32

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Rings_Burst_Smokey.part
		Lifespan 	10
	End

	Event
		Type	PositOnly
		At	Chest
		Part	:Rings_Burst_Rapid.part
		Lifespan 	15
	End
End

Condition
	On	Time
	Time	48

	Event
		EName	Prime
		Type	Start
		At	Chest
		BhvrOverride
			Alpha		150
			Trackrate	2.75
			TintGeom	1
			FadeInLength	5
			FadeOutLength	5
		End
		Part	:Gust_Streaks_Continuing.part
		Part	:Rings_Projectile_LocalFacing.part
		Part	:Rings_Shockwave_Smokey_Continuing.part
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On	Time
	Time	50

	Event
		EName	RingsAnchor
		Type	Local
		At	Prime
	End
End

Condition
	On	Cycle
	Time	2

	Event
		EName	Prime2
		Type	Local
		At	RingsAnchor
		BhvrOverride
			PyrRotate	90 0 0
			PyrRotateJitter	20 20 20
		End
		Part	:Rings_Rising_LocalFacing.part
		Lifespan 20
	End

	Event
		EName	Prime3
		Type	Local
		At	RingsAnchor
		BhvrOverride
			PyrRotate	90 0 0
			PyrRotateJitter	20 20 20
		End
		Part	:Rings_Rising_LocalFacing_Smokey.part
		Lifespan 20
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Prime2
		Type	Destroy
	End

	Event
		EName 	Prime3
		Type	Destroy
	End
End


#############################################################

End