#########################################################
##	forceBubble
########################################################
FxInfo


LifeSpan	160	#95

########################################################



Condition
	On Time
	Time 0

	Event
		Type 	Start
		At	Origin
		Sound ReichsmanFistOfTyranny_01 180 180 .88
	End
End


Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	WepR

		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part

		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:HandBeams.part
		Part5	:HandBeamsSub.part





		LifeSpan	33


	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	local
		At	root

		Bhvr	Behaviors/StatesmanCameraShake.bhvr

		Part	:ElectricityControl01Lrg.part
		Part	:ElectricityControl02Lrg.part

		Part1	POWERS/ElectricityControl/InfernorockBits.part
		Part2	Explosions/FireExplosion/ExplosionDarkStreams.part
		Part3	Explosions/FireExplosion/DarkInferno.part
		Part4	:YellowStarCore.part

		LifeSpan	5


	End

End

Condition
	On Time
	Time 45

	Repeat	4

	Event
		EName	Sparkfar
		Type 	Start
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\Reichsman_LightningHit.fx

		Lifespan	90

	End

End

#####################################################
##Phs Hand sparks
##############################

Condition
	On Time
	Time 42

	Repeat	9

	Event
		EName	Sparkfar
		Type 	Start
		At	WepR
		ChildFX	V_COV\PhysicsEnabled\Reichsman_ElectricSpark.fx

		Lifespan	90

	End

End

Condition
	Time 	40

	Event
		EName 	Mallet1
		Type	local
		At	WepR
		bhvr	Weapons/Bow/Trickarrow/EMPBubbleScale2.bhvr
		BhvrOverride
			RGB0 0 0 255
		End
		Geom	GEO_WhiteSphere

		LifeSpan	5

	End

####################################################
##EneergyCracks
############################



	Event
		EName 	Primex
		Type	local
		At	root

		Bhvr	Behaviors/StatesmanCameraShake.bhvr

		Part	:ElectricityTendrilsWhite.part
		Part	:YellowElectricityTendrils.part
		Part	:YellowElectricityTendril.part
		LifeSpan	13
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	root
		Bhvr	Behaviors/StatesmanCameraShake.bhvr
		Part	:YellowElectricityCracksFlat.part
		Part	:ElectricityCracksFlatDark.part
		LifeSpan	2
	End

End