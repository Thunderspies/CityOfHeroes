#########################################################
##	
##

FxInfo

LifeSpan  345


Flags InheritAnimScale

##################################


Condition
	On 	Time
	Time 	0

	Event
		EName   Welder
		Type	Start
		At	Root
		Anim	FX_RoboHatch_upgrade
		Bhvr	Behaviors/GenericParticleFade.bhvr
                        BhvrOverride
                                    Behavior
                                    FadeOutLength	15
			End
	End

	Event
		EName   Welder_node
		Type	Local
		At	Welder
		AnimPiv	RingL  ##F1_L
	End
	
End

#####  Sound rise up ############################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		Sound techdoor3 80 80 .56
	End

End


Condition
	On 	Time
	Time 	90

	Event
		Type	Start
		At	Root
		Sound Robotupgrade5b 80 80 .86
	End

End

Condition
	On 	Time
	Time 	260

	Event
		Type	Start
		At	Root
		Sound Robotupgrade6 80 80 .86
	End

End


######  Welder Sparks 1  #####################################


Condition
	On 	time
	Time 	120

	Event
		Type		Local 
		At 		Welder_node
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	20
		
	End

End


Condition
	On 	time 
	Time 	121

	Event
		Type		Local 
		At 		UlegL
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	20
		
	End

End

Condition
	On 	time 
	Time 	121

	Event
		Type		Local 
		At 		LlegL
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	20
		
	End

End



######  Welder Sparks 2  #####################################


Condition
	On 	time
	Time 	194

	Event
		Type		Local 
		At		Welder_node
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	20
		
	End

End

Condition
	On 	time
	Time 	194

	Event
		Type		Local 
		At		UarmR
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	20
		
	End

End

Condition
	On 	time
	Time 	194

	Event
		Type		Local 
		At		Col_R
		ChildFx		World/Ruins/StreetSparks.fx
		LifeSpan	20
		
	End

End


End				