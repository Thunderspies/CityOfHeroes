#############################################################
## Child_OrbitalLance_Attack.fx
#############################################################

FxInfo
LifeSpan 60

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	MissileOffset
		Type	PositOnly
		At	Origin
		BhvrOverride
			PositionOffset	0.0 100.0 0.0
		End
	End

	Event
		EName 	Prime
		Type	Start
		At	MissileOffset
		BhvrOverride
			InitialVelocity		0.0 -5.0 0.0
			InitialVelocityJitter	0.05 0.0 0.05
		End
		Part	:Projectile_Head_Ember_Small.part
		Part	:OrbitalLance_Trail_Tendril.part
		Part	:OrbitalLance_Trail_Glow.part
		Part	:Projectile_Head_Ember.part
		Magnet	Root
		LookAt	Root
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
	On 	Time
	Time 	21

	Event
		Type	PositOnly
		At	Origin
		Bhvr	behaviors\CameraShake01Subtle.bhvr

		BhvrOverride
			PositionOffset	0.0 0.2 0.0
		End
		Part	:Explosion_Flash_Large.part
		Part	:Explosion_Glow.part
		Part	:Explosion_Shockwave_Large.part
		Part	:Explosion_Glow_Lingering.part
		Part	:Explosion_Streaks.part
		Part	:Explosion_Fire_Rings_Large.part
		Part	:Explosion_Flash_Large_Flat.part
		Part	:Explosion_Glow_Lingering_Flat.part
		Part	:Explosion_Shockwave_Large_Flat.part
		Part	:Explosion_Glow_Flat.part
		Lifespan 4
	End
End

Condition
	On	Time
	Time	24

	Event
		Type	PositOnly
		At	Origin
		Part	:Explosion_Streaks_Thin.part
		Part	:Explosion_Sparks.part
		Part	:Explosion_Smoke.part
		Part	:Explosion_Smoke_Lingering.part
		Part	:Explosion_Fire_Rings.part
		Part	:Explosion_Smoke_Additive.part
		Lifespan 3
	End


End

#############################################################

End