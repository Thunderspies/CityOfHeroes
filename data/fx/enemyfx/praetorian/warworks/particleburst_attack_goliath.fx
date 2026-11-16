#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound WW_ParticleBurst_Attack 200 200 .8
	End

	Event
		EName	PlasmaAnchorR
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	3.0 0.7 0.0
		End
		Part	:Muzzle_Glow_Blue.part
		Part	:Muzzle_Tendril_Small.part
		Lifespan	50
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	3.0 0.7 0.0
		End
		Part	:Muzzle_Rings_Faint.part
		Part	:Muzzle_Tendril_Fast.part
		Sound WW_ParticleBurst_Particle_01 200 200 .8
		Lifespan	45
		LifespanJitter	5
	End
End

Condition
	On 	Time
	Time 	28

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	3.0 0.7 0.0
		End
		Part	:Muzzle_Flash.part
		Part	:Muzzle_Flash_Shockwave.part
		Sound WW_ParticleBurst_Particle_02 200 200 .8
		Lifespan	1
	End
End


Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime
		Type	Start
		At	PlasmaAnchorR
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			InitialVelocityJitter	2.0 1.0 2.0
			TrackRate		6.0
		End
		ChildFX	:Child_ParticleBurst_Goliath.fx
		Sound WW_ParticleBurst_Particle_03 200 200 .8
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

End