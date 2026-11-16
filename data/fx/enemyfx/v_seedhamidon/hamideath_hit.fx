#############################################################
## HamiDeath_Hit.fx
#############################################################

FxInfo
Lifespan 250
#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SeedOfHamidon_Hit_01 2000 1000 .8
	End
End


Condition
	On	Time
	Time	0

	Event
		Type	Start
		CAMERASPACE
		#Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Slime_Camera.part
	End
End

Condition
	On	Time
	Time	0
	Repeat 2

	Event
		EName	BodySlime
		Type	Local
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		ChildFX	:HamiDeath_Hit_Child.fx
		Lifespan 160
	End
End
############################################################
End