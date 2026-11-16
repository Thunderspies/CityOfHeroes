#########################################################
##	template

FxInfo

LifeSpan	300

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound stumphit4 80 80 .78
	End
End




Condition
	Time 	3
	Repeat	55
	RepeatJitter	5

	Event
		Type	start
		At	chest
		Geom	woodchip
		Bhvr	Behaviors\StartOffset.bhvr
		BhvrOverride
			StartColor		110 110 160
			Scale			.05 .05 .05
			ScaleRate		1.1 1.1 1.1
			EndScale		.05 .05 .05
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.15 0.15 0.15
			PyrRotateJitter		180 180 180
			Drag			0.004
			physics 		1
			physRadius 		.8
			physGravity 		1
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			SpinJitter		.1 .1 .1
		End
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End

End


Condition

	Event
		Type	Start 
		At	Chest
		
#		BhvrOverride
#		
#			spinjitter	8
#		
#		End

		Part	Explosions/WoodExplosion/SawDust01.part
		Part	Explosions/WoodExplosion/SawDust02.part
#		Part	Explosions/WoodExplosion/SawdustLarge.part

		LifeSpan 5

	End

	Event
		Type	StartPositOnly 
		At	Chest

		BhvrOverride
			Scale	1.3 1.3 1.3
		End

		BHVR   :StumpHitLeft.bhvr
		Geom  Root_throw_Piece01      
		#Geom   Bottle1     
		
		LifeSpan 2000

	End

	Event
		Type	StartPositOnly 
		At	Chest
		
		BhvrOverride
			Scale	1.3 1.3 1.3
		End

		BHVR   :StumpHitRight.bhvr
		Geom   Root_throw_Piece02        
		#Geom   Bottle1
		LifeSpan 2000

	End

End

End		