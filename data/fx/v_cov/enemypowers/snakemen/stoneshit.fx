#########################################################
##	
##
FxInfo


Input  
	InpName	chest
End

LifeSpan 100

####################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	chest
		
		Part1	Powers/EarthControl/StoneHandsDustHit.part
		Part2	Powers/EarthControl/StoneHandsDustHit2.part
#		Bhvr	Behaviors/CameraShake01Subtle.bhvr
		Sound stonehit 80 80 .9
		
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	11

	Event
		Type	Local 
		At	chest
		
		BhvrOverride
			
#			Scale			1 1 1
			StartColor		255 180 220

		End

		ChildFX		V_COV\PhysicsEnabled\RockDebrisDarkScatter.fx

		Lifespan	100
		
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1
	RepeatJitter	1

	Event
		Type	Local 
		At	chest
		
		BhvrOverride
			
#			Scale			1 1 1
			StartColor		255 180 220

		End

		ChildFX		V_COV\PhysicsEnabled\RockChunkBrownScatter.fx

		Lifespan	90
		LifespanJitter	15
		
	End

End


End			