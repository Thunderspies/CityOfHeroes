#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 300

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
		At	C_Head

		Part2	:Muzzle_Tendril.part
		Part2	:Muzzle_Tendril_Streak.part
		Part2	:Muzzle_Smoke.part
		Part2	:Muzzle_Glow.part


		PMagnet	WepR
		Lifespan 30
	End


End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Root
		sound Plasma_Ray 200 200 .8
	End

End

Condition
	On 	Time
	Time 	20



	Event
		EName 	HandGlowRight2
		Type	Local
		At	C_Head
		Part1	:Muzzle_Blast_Rings.part
		PMagnet	C_Head
		Lifespan 25

	End

	Event
		EName 	HandGlowRight2
		Type	Local
		At	C_Head
		Part1	:Muzzle_Tendril_Core.part
		Part2	:Muzzle_Glow.part

		PMagnet	C_Head
		Lifespan 20

	End

	Event
		Ename	Flash
		Type	StartPositOnly
		At	C_Head

		Part	:Muzzle_Trail_Core.part

		Lookat 	Target
		PMagnet	Target

		Lifespan 25

	End
End

#############################################################
Condition
	On	Time
	Time	30



	Event
		EName 	BeamGeo
		Type	Start
		At	C_Head
		Bhvr	behaviors/InfernoBoltBeamSmall.bhvr
		BhvrOverride
			StartColor	0 0 255

			FadeInLength		5
			FadeOutLength		10

			Scale		0.1 0.1 0.007
			ScaleRate	.05 .05 .05
 			EndScale	.35 .35 0.2
		End

		geom	Tintable_FireBlast1
		Part	:Beam_Trail_Ember.part

		Magnet	Target
		LookAt	Target
		POther	Target
		LifeSpan		35
	End

	Event
		Ename	BeamHit
		Type	Local
		At	Target

		Part	:Hit_Beam_Flash_Blue.part
		Part	:Muzzle_Buildup_Flash_Highlight.part
		Lifespan	20
	End
End

Condition
	On 	Time
	Time 	45

	Event
		EName 	Flash
		Type	Start
		At	C_Head
		Part	:Muzzle_Flash.part
	End

	Event
		EName 	Prime
		Type	StartPositOnly
		At	C_Head

		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			Scale		2.5 2.5 10.0
			TrackRate	5.0
			StartColor	100 100 255
		End

		Part	:Beam_Trail_Ember_Small.part
		Part	:Projectile_Trail_Tendril.part
		Part	:Projectile_Trail_Rings_Blue.part
		Part	:Projectile_Trail_Steam_Small.part
		Part	:Projectile_Trail_Glow.part
		Geom	ForceRepulsion03

		Magnet	Target
		LookAt	Target
		Lifespan	80
	End

End


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End