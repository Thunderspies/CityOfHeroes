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
			PositionOffset	2.4 0.7 0.0
		End
		Part	:Muzzle_Glow_Blue.part
		Part	:Muzzle_Tendril_Small.part
		Lifespan	50
	End

	Event
		EName	PlasmaAnchorL
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-2.4 0.7 0.0
		End
		Part	:Muzzle_Glow_Blue.part
		Part	:Muzzle_Tendril_Small.part
		Lifespan	50
	End

End

Condition
	On 	Time
	Time 	35

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
		Part	:Muzzle_Rings_Faint.part
		Part	:Muzzle_Tendril_Fast.part
		Sound WW_ParticleBurst_Particle_01 200 200 .8

		Lifespan	45
		LifespanJitter	5
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr

		BhvrOverride
			PositionOffset	-2.4 0.7 0.0
		End
		Part	:Muzzle_Rings_Faint.part
		Part	:Muzzle_Tendril_Fast.part
		Lifespan	45
		LifespanJitter	5

	End

End

Condition
	On 	Time
	Time 	38

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.4 0.7 0.0
		End
		Part	:Muzzle_Flash.part
		Part	:Muzzle_Flash_Shockwave.part
		Sound WW_ParticleBurst_Particle_02 200 200 .8
		Lifespan	1
	End
End

Condition
	On	Time
	Time	40
	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr

		BhvrOverride
			PositionOffset	-2.4 0.7 0.0
		End
		Part	:Muzzle_Flash.part
		Part	:Muzzle_Flash_Shockwave.part
		Sound WW_ParticleBurst_Particle_03 200 200 .8
		Lifespan	1
	End

End

Condition
	On 	Time
	Time 	40

	Event
		EName 	Prime
		Type	Start
		At	PlasmaAnchorR
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			InitialVelocityJitter	2.0 1.0 2.0
			TrackRate		6.0
		End
		ChildFX	:Child_ParticleBurst_Right.fx
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On 	Time
	Time 	43

	Event
		EName 	Prime2
		Type	Start
		At	PlasmaAnchorL
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			InitialVelocityJitter	2.0 1.0 2.0
			TrackRate		6.0
		End
		ChildFX	:Child_ParticleBurst_Left.fx
		Magnet	Target
		LookAt	Target
		Sound WW_ParticleBurst_Particle_05 200 200 .8
	End
End

#############################################################

End