#########################################################
##	template

FxInfo

LifeSpan		75

#########################################################
##Arrows Hitting
###########################################################################

Condition
	On Time 
	Time 5
	
	Event
		EName	Arrow1hookup
		Type 	local
		At	Root

		BhvrOverride	
			PositionOffset	0 7 1
		End

	End

	Event
		EName	Arrow1
		Type 	create
		Inherit	Position rotation Scale
		Update	 rotation Position
		At	Arrow1hookup

		BhvrOverride	
			
Gravity			0.07 
#Initialvelocity		0 0 .5
 
PhysScale		2 2 2
Drag			0.006
physics 		1
physRadius 		0.8
physGravity 		.9
physRestitution 	0.5
physSFriction 	 	0.3
physDFriction 	 	0.3
#FadeOutLength		1110
PhysDebris		3
PhysDensity		1

			PyrRotateJitter	-10 0 -10
			StartJitter	1 .6 1
		End
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		LifeSpan	5
		
		Geom	aNormalArrow
	End

End
	
################################################################################

Condition
	On Time 
	Time 18
	
	Event
		EName	Arrow2hookup
		Type 	local
		At	Root

		BhvrOverride	
			PositionOffset	0 7 -1
		End

	End

	Event
		EName	Arrow2
		Type 	create
		Inherit	Position rotation Scale
		Update	 rotation Position
		At	Arrow1hookup

		BhvrOverride	
			
Gravity			0.07 
#Initialvelocity		0 0 .5

PhysScale		2 2 2
Drag			0.006
physics 		1
physRadius 		0.8
physGravity 		.9
physRestitution 	0.5
physSFriction 	 	0.3
physDFriction 	 	0.3
#FadeOutLength		1110
PhysDebris		3
PhysDensity		1

			PyrRotateJitter	-10 0 -10
			StartJitter	1 .6 1
		End
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		LifeSpan	5

		Geom	aNormalArrow
	End

End
	
################################################################################

Condition
	On Time 
	Time 8
	
	Event
		EName	Arrow3hookup
		Type 	local
		At	Root

		BhvrOverride	
			PositionOffset	-2 3 0
		End

	End

	Event
		EName	Arrow3
		Type 	create
		Inherit	Position rotation Scale
		Update	 rotation Position
		At	Arrow3hookup

		BhvrOverride	
			
Gravity			0.07 
#Initialvelocity		0 0 .5
 
PhysScale		2 2 2
Drag			0.006
physics 		1
physRadius 		0.8
physGravity 		.9
physRestitution 	0.5
physSFriction 	 	0.3
physDFriction 	 	0.3
#FadeOutLength		1110
PhysDebris		3
PhysDensity		1

			PyrRotateJitter	-10 0 -10
			StartJitter	1 .6 1
		End
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		LifeSpan	5
		
		Geom	aNormalArrow
	End

End
	
################################################################################

Condition
	On Time 
	Time 28
	
	Event
		EName	Arrow4hookup
		Type 	local
		At	Root

		BhvrOverride	
			PositionOffset	2 4 0
		End

	End

	Event
		EName	Arrow4
		Type 	create
		Inherit	Position rotation Scale
		Update	Position rotation 
		At	Arrow4hookup

		BhvrOverride		
			
Gravity			0.07 
#Initialvelocity		0 0 .5
 
PhysScale		2 2 2
Drag			0.006
physics 		1
physRadius 		0.8
physGravity 		.9
physRestitution 	0.5
physSFriction 	 	0.3
physDFriction 	 	0.3
#FadeOutLength		1110
PhysDebris		3
PhysDensity		1

			PyrRotateJitter	-10 0 -10
			StartJitter	1 .6 1
		End
		
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		LifeSpan	5
		
		Geom	aNormalArrow
	End

End

################################################################################

Condition
	On time
	Time 0

	Event

		Type	Local
		At	Chest
		Sound aim2 60 60 .6
		End
end




Condition
	On Time 
	Time 0
	
	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	:YellowUpGlow.part
		Part2	:YellowUpSpecks2.part
		#Sound	clawYellow3 100 30 .8
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

End

Condition
	On Time 
	Time 10
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:YellowUpSpecks.part
		Part2	:YellowUpRing01.part
		Part3	:YellowUpRing01a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 20
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:YellowUpSpecks.part
		Part3	:YellowUpRing02.part
		Part4	:YellowUpRing02a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 30
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:YellowUpSpecks2.part
		Part2	:YellowUpRingSlow.part
		Part3	:YellowUpRing03.part
		Part4	:YellowUpRing03a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
	End

End


Condition
	On Time 
	Time 13

	Event
		EName	pow2
		Type 	Destroy
			
	End

End

	

End	

	