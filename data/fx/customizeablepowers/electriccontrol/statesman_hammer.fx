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
		Sound Statesmanhammer2 180 180 .88
	End
End


Condition
	On 	Time
	Time 	10

	Event
		Type	local 
		At	WepR
		
		Part	:YellowElectricityControl01.part
		Part	:YellowElectricityControl02.part
		Part	:YellowElectricityInnerGlow.part
		Part	:YellowElectricityOuterGlow.part
		Part	:YellowElectricityArch.part
		Part	:YellowElectricityRing.part
		
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

		Part1	:InfernorockBits.part
		Part	Explosions/FireExplosion/ExplosionlightStreams.part
		Part2	Explosions/FireExplosion/YellowInferno.part
		Part	:YellowStarCore.part

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
		ChildFX	V_COV\PhysicsEnabled\StatesmanLightningHit.fx

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
		ChildFX	V_COV\PhysicsEnabled\StatesmanElectricSpark.fx

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
		Geom	SuperStrengthBubble
		
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
#		Part	:InfernorockBits2.part
		LifeSpan	13
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	root
		Bhvr	Behaviors/StatesmanCameraShake.bhvr
		Part	:YellowElectricityCracksFlat.part
		Part	:ElectricityCracksFlatWhite.part
		Part	:YellowElectricityTendrilsFlat.part
		Part	:YellowElectricityTendrilFlat.part
		Part	:YellowElectricityTendrilBase.part
		LifeSpan	2
	End

End