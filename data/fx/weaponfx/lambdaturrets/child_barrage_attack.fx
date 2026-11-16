#############################################################
## Child_OrbitalLance_Attack.fx
#############################################################

FxInfo
LifeSpan 150

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	ImpactOffset
		Type	StartPositOnly
		At	Origin
	End

	Event
		EName 	MissileOffset
		Type	StartPositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0.0 60.0 0.0
		End
	End

	Event
		EName 	Prime
		Type	StartPositOnly
		At	MissileOffset
		BhvrOverride
			InitialVelocity		0.0 -7.0 0.0
			InitialVelocityJitter	0.05 0.0 0.0
		End
		Part	:Grenade_Glow_Chemical.part
		Part	:Grenade_Globs_Chemical.part
		Part	:Grenade_Trail_Smoke_Chemical.part
		Part	:Grenade_Trail_Smoke_Chemical_Full.part
		Geom	_5th_missile
	#	Magnet	Root
		LookAt	ImpactOffset
		Lifespan 15
	End
End

Condition
	On	Time
	Time	7


	Event
		Type	PositOnly
		At	Origin
		SoundNoRepeat 2
		Sound WW_OrbitalLance_Hit_01 200 200 .8
		Sound WW_OrbitalLance_Hit_02 200 200 .8
		Sound WW_OrbitalLance_Hit_03 200 200 .8
		Sound WW_OrbitalLance_Hit_04 200 200 .8
		Sound WW_OrbitalLance_Hit_05 200 200 .8
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	ImpactOffset
		Part	:Flash_Beam.part
		Part	:Flash_Beam_Highlight.part
		Part	:Flash_Beam_Smoke.part
		Part	:Flash_Beam_light.part
		Lifespan	12
	End

	Event
		Type	Local
		At	ImpactOffset
		Part	:Flash_Circle.part
		Part	:Flash_Circle_Large.part
		Part	:Flash_Circle_Fire.part
		Lifespan	2
	End

	Event
		Type	Local
		At	ImpactOffset
		ChildFX	:Shell_Hit.fx
		Lifespan	100
	End




End

#############################################################

End