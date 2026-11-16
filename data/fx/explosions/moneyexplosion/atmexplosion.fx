#########################################################
##	onfire
##
FxInfo  

LifeSpan	800


Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		ChildFX	Explosions\CashExplosion\cashExplosion.Fx
		Lifespan	600
	End

############################################################
##Sound####
##################

#	Event
#		Type	start 
#		At	chest
#		
#		Sound	CoinExplo2 80 30 .7
#		LifeSpan	5
#	End
#
#
#
#############################################################
###Fx####
###################
#
#End
#
#Condition
#	Time 	0
#	Repeat	21
#
#	Event
#		Type	start 
#		At	chest
#		
#		Bhvroverride
#			InitialVelocity		0.0 0.28 0.0		
#			InitialVelocityJitter	0.15 0.2 0.15
#			Drag			0.004
#			physics 		1
#			physRadius 		.5
#			physGravity		1.2
#			physRestitution 	0.5
#			physSFriction 	 	0.5
#			physDFriction 	 	0.9
#			FadeOutLength		100
#		End
#
#		Part	:QuarterHeads.part
#	End
#
#End
#
#Condition
#	Time 	0
#	Repeat	21
#
#	Event
#		Type	start 
#		At	chest
#		
#		Bhvroverride
#			InitialVelocity		0.0 0.28 0.0		
#			InitialVelocityJitter	0.15 0.2 0.15
#			Drag			0.004
#			physics 		1
#			physRadius 		.5
#			physGravity		1.2
#			physRestitution 	0.5
#			physSFriction 	 	0.5
#			physDFriction 	 	0.9
#			FadeOutLength		100
#		End
#
#		Part	:QuarterTails.part
#	End

End


End			