#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

LifeSpan 180

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		sound Antimatter_Beam 200 200 .8
	End

	Event
		Ename	Flash
		Type	Local
		At	C_Eyes
		Part	:Muzzle_Buildup_Flash_Highlight.part
		Lifespan 70
	End
End

Condition
	On	Time
	Time 	15

	Event
		EName 	BeamGeo
		Type	PositOnly
		At	C_Eyes
		Bhvr	behaviors\InfernoBoltBeamSmall.bhvr
		BhvrOverride
			StartColor		255 10 0
			FadeInLength		5
			FadeOutLength		10
			Scale			0.1 0.1 0.007
			ScaleRate		0.05 0.05 0.05
 			EndScale		0.35 0.35 0.2
		End
		Part	:Muzzle_Buildup_RingSpots.part
		Part	:Muzzle_Buildup_RingSpots_Alpha.part
		geom	Tintable_FireBlast1
		Magnet	Target #Destination
		LookAt	Target #Destination
		POther	Target #Destination
		LifeSpan 65
	End

	Event
		Ename	BeamHit
		Type	Local
		At	Target
		ChildFX	:Android_Antimatter_Beam_Hit2.fx
		Lifespan 90
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		Ename	Flash
		Type	PositOnly
		At	C_Eyes
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Muzzle_Buildup_RingSpots_Large.part
		Part	:Muzzle_Buildup_Flash_Highlight.part
		Part	:Muzzle_SlowBuildup_Flash.part
		Lifespan 25
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	PositOnly
		At	C_Eyes
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Beam_Glow_Additive.part
		Part	:Muzzle_Buildup_Rings.part
		Magnet	Target #Destination
		LookAt	Target #Destination
		POther	Target #Destination
		LifeSpan 25
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	PositOnly
		At	C_Eyes
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Beam_Glow.part
		Magnet	Target #Destination
		LookAt	Target #Destination
		POther	Target #Destination
		LifeSpan 20
	End
End

#############################################################

Condition
	On	Time
	Time	70

	Event
		Ename	FlashRing
		Type	StartPositOnly
		At	C_Eyes
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Muzzle_Release_Flash.part
		Part	:Muzzle_Release_Rings.part
		Lifespan 5
	End
End

Condition
	On 	Time
	Time 	70

	Event
		Ename 	Prime
		Type	Start
		At	C_Eyes
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		9.0
		End
		Part	:Projectile_Trail_Rings_Subtractive.part
		Part	:Beam_Trail_Ember_Additive.part
		Part	:Beam_Trail_Ember_Subtractive.part
		Part	:Beam_Trail_Tendril.part
		Part	:Beam_Trail_Glow.part
		Part	:Beam_Trail_Tendril_Subtractive.part
		Part	:Projectile_Trail_Glow_Subtractive.part
		PMagnet	Target #Destination
		Magnet	Target #Destination
		LookAt	Target #Destination
		Lifespan 80
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End