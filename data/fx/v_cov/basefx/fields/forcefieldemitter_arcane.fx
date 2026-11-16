#########################################################
##	forceBubble
########################################################
FxInfo



###############  Sounds   ######################################

Condition
	On	Time	
	Time 	7

	Event
		Type 	Local
		At	Origin
		Sound forcehands3 80 80 .4
	End

End


Condition
	On	Time	
	Time 	18

	Event
		Type 	Local
		At	Origin
		Sound ForceField3_loop 100.0 100.0 .42
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End

Condition
	On	Time	
	Time 	18

	Event
		Type 	Local
		At	Origin
		Sound ForceField6_loop 100.0 100.0 .26
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End

#####################  Eye Glows  ######################

Condition
	On 	Cycle
	Time 	15

	Event
		EName 	Prime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.75 4.9 4.4
		End
		Part1	\POWERS\ForceField\HandGlowsc.part	
		LifeSpan	5
	End	

End

Condition
	On 	Cycle
	Time 	15

	Event
		EName 	Prime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-0.75 4.9 4.4
		End
		Part1	\POWERS\ForceField\HandGlowsc.part	
		LifeSpan	5 
	End	

End

#####################  AoE  ###################################


Condition
	On 	Time
	Time 	0
	Event
		EName 	Center
		Type	Local 
		At	Root
		BhvrOverride
			PositionOffset	0 4 0
		End
			
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	PositOnly
		At	Center
		Bhvr 	:ForceBubbleLarge2.bhvr

		Part1	powers/forcefield/bubblesphereLarge.part
		
		#LifeSpan	30
		
	End

	Event
		EName 	Prime2
		Type	PositOnly 
		At	Center
		Bhvr 	:ForceBubbleLarge.bhvr
		
		Geom	FX_ForceBubble
		
		#LifeSpan	50
				
	End

	Event
		EName 	Ring
		Type	PositOnly 
		At	Root
		Part1	:ForceBubbleRing2.part
				
	End

	Event
		EName 	Ring
		Type	PositOnly 
		At	Root
		Part1	:ForceBubbleRingBurst2.part
				
	End

End

End
