#############################################################
## Child_FinalExplosion.fx
#############################################################

FxInfo

Lifespan 500

## PHYSICS FORCES ###########################################################

Condition
	On	Time
	Time	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		Bhvr	:CameraShake_Nuke.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		50
			PhysForcePower		1250
			PhysForcePowerJitter	250
		End
		Lifespan 15
	End
End

## GROUND EXPLOSION ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Primex
		Type	Local
		At	Root
		Part	:InfernorockBits1.part
		Lifespan 15
	End

	Event
		Type	Local
		At	Root
		Bhvr	:BlastRingScale.bhvr
		Part1   :Infernoflash.part
		Part2   :Infernoflash2.part
	End

	Event
		Type	Posit
		At	Root
		Bhvr 	:BlastScaler.bhvr
		Geom	GEO_WhiteSphere #CosmicPeaceBringerShield
		LifeSpan 10
	End
End

Condition
	On	Time
	Time	0

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	:PeaceBringerTest.bhvr
	End

	Event
		Type	Local
		At	RadiusFxScale
		Bhvr	:Infernobubble.bhvr
		##Part	:NovaSmoke01.part
		Part	:NovaSmoke02.part
		Part	:NukeFlash.part
		Lifespan 25
	End
End

Condition
	On 	Time
	Time 	2

	Event
		Type	start
		At	RadiusFxScale
		bhvr	:BlastRingScale2a.bhvr
		geom	MegaRing2
		LifeSpan 2
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	Root
		Part2	:Inferno.part
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	11

	Event
		Type	Local
		At	Root
		Part2	:Inferno1.part
		Part	:InfernorockBits2.part
		Lifespan 14
	End
End

Condition
	On 	Time
	Time 	14

	Event
		Type	Local
		At	Root
		Part2	:Inferno2.part
		Lifespan 18
	End
End

Condition
	On 	Time
	Time 	17

	Event
		Type	Local
		At	Root
		Part2	:Inferno3.part
		Part	:InfernorockBits3.part
		Lifespan 22
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	Root
		Part2	:Inferno4.part
		Lifespan 26
	End
End

## MUSHROOM CLOUD ###########################################################

Condition
	On	Time
	Time	0

	Event
		EName   RadiusFxScale
		Type	Local
		At	Root
		ChildFX	:Child_MushroomCloud.fx
	End
End

## SPLATS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Root
		Bhvr	:Splat_Crater.bhvr
		Splat	StoneCracks.tga
		Lifespan 300
	End

#	Event
#		Type	Start
#		At	Root
#		Bhvr	:Splat_Glow.bhvr
#		Splat	Ember02.tga #Glow.tga
#		Lifespan 60
#	End

	Event
		Type	Start
		At	Root
		Part	:Decal_CraterGlow1.part
		Lifespan 15
	End
End

#############################################################

End