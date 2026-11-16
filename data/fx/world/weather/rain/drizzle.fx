#########################################################
##	forceBubble
########################################################
FxInfo



Condition
	On 	cycle
	Time 	2

	Event	
		Type	posit
		At	root

#		BhvrOverride
#		
#			physics 		1
#			physRadius 		2.8
#			physGravity 		.1
#			physRestitution 	0.5
#			physSFriction 	 	0.3
#			physDFriction 	 	0.5
#			PositionOffset		0 70 0
#			StartJitter		60.0 10.0 60.0
#		End
#

		ChildFx	WORLD\Weather\Rain\RainDrop.fx

		
		LifeSpan	75 
	End

End

Condition
	On 	cycle
	Time 	7

	Event	
		Type	posit
		At	root

#		BhvrOverride
#		
#			physics 		1
#			physRadius 		2.8
#			physGravity 		.1
#			physRestitution 	0.5
#			physSFriction 	 	0.3
#			physDFriction 	 	0.5
#			PositionOffset		0 70 0
#			StartJitter		60.0 20.0 60.0
#		End


		ChildFx	WORLD\Weather\Rain\RainDrop.fx

		
		LifeSpan	75 
	End

End

Condition
	On 	cycle
	Time 	3
	Chance	.3

	Event	
		Type	posit
		At	root

		BhvrOverride

			PositionOffset		0 70 0
			StartJitter		60.0 20.0 60.0
		End


		ChildFx	WORLD\Weather\Rain\RainDrop.fx

		
		LifeSpan	75 
	End

End

Condition
	On 	cycle
	Time 	13

	Event	
		Type	posit
		At	root

		BhvrOverride
			PositionOffset		0 70 0
			StartJitter		100.0 20.0 100.0
		End


		ChildFx	WORLD\Weather\Rain\RainDrop.fx

		
		LifeSpan	75 
	End

End

End