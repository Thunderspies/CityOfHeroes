#############################################################
## SeedHamidon_Aura.fx
#############################################################

FxInfo
#Lifespan 200

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	T_Root
		Sound 	SeedOfHamidon_Hit_01 2000 1000 .8
	End
	Event
		Type	Local
		At	Root
		Sound 	SeedOfHamidon_LowHealth_Loop 1000 1000 .6
	End

End

Condition
	On 	Time
	Time 	0 #40

	Event
		Ename	TailBurst6
		Type	Local
		At	T_Rwing4
		BhvrOverride
			PositionOffset	0 0 -1.5
			Scale	0.0005 0.0005 0.0005
		End
		ChildFX	:SeedHamidon_TailLowHealth_Child.fx
		#Lifespan 55
	End
End

Condition
	On 	Time
	Time 	0 #50

	Event
		Ename	TailBurst5
		Type	Local
		At	T_Rwing4
		BhvrOverride
			Scale	0.0005 0.0005 0.0005
		End
		ChildFX	:SeedHamidon_TailLowHealth_Child.fx
		#Lifespan 50
	End
End

Condition
	On 	Time
	Time 	0 #55

	Event
		Ename	TailBurst4
		Type	Local
		At	T_Rwing3
		BhvrOverride
			Scale	0.001 0.001 0.001
		End
		ChildFX	:SeedHamidon_TailLowHealth_Child.fx
		#Lifespan 45
	End
End

Condition
	On 	Time
	Time 	60

	Event
		Ename	TailBurst3
		Type	Local
		At	T_Rwing2
		BhvrOverride
			Scale	0.01 0.01 0.01
		End
	#	ChildFX	:SeedHamidon_Death_Child.fx
		ChildFX	:SeedHamidon_TailLowHealth_Child.fx
		#Lifespan 40
	End
End

Condition
	On 	Time
	Time 	65
#
	Event
		Ename	TailBurst2
		Type	Local
		At	T_Rwing1
		BhvrOverride
			PositionOffset	0 50 0
			Scale	0.01 0.01 0.01
		End
	#	ChildFX	:SeedHamidon_Death_Child.fx
		ChildFX	:SeedHamidon_TailLowHealth_Child.fx
		#Lifespan 35
	End

	Event
		Ename	TailBurst1
		Type	Local
		At	T_Chest
		BhvrOverride
			PositionOffset	-1.8 -0.2 -0.6
			Scale		0.05 0.05 0.05
		End
	#	ChildFX	:SeedHamidon_HeadDeath_Child.fx
		ChildFX	:SeedHamidon_BodyLowHealth_Child.fx
		#Lifespan 35
	End

	Event
		Ename	TailBurst1b
		Type	Local
		At	T_Chest
		BhvrOverride
			PositionOffset	1.8 -0.2 -0.6
			Scale		0.05 0.05 0.05
		End
	#	ChildFX	:SeedHamidon_HeadDeath_Child.fx
		ChildFX	:SeedHamidon_BodyLowHealth_Child.fx
		#Lifespan 35
	End
End

Condition
	On 	Time
	Time 	70

	Event
		Type	Local
		At	T_Chest
		Bhvr	behaviors/FadeOut.bhvr
		BhvrOverride
			#PositionOffset	0 -0.3 1
			Scale		0.1 0.1 0.1
		End
	#	ChildFX	:SeedHamidon_HeadDeath_Child.fx
		ChildFX	:SeedHamidon_BodyLowHealth_Child.fx
		#Lifespan 30
	End
End

Condition
	On 	Time
	Time 	85

	Event
		Type	Local
		At	T_Chest
		Bhvr	behaviors/FadeOut.bhvr
		BhvrOverride
			PositionOffset	0 -0.3 1
			Scale		0.1 0.1 0.1
		End
	#	Part	:Splash_InnerSplatter.part
		#Lifespan 15
	End
End

#############################################################
End