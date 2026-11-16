#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound Fast 80 80 .8
	End
End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition

	On	Time
	Time	22

	Event
		Ename	Prime
		Type	start 
		At	chest
#		bhvr	behaviors/projectile3.bhvr
#		geom	centerdummy
#		
#		magnet	target
#		lookat	target
		Sound graniteexplo 88 88 .77

	End

	Event

		Type	Local 
		At	mystic
		Part	:ConeBlackBits.part
		Part	:ConeWhiteSmoke.part

		LifeSpan	10
	End

#	Event
#
#		Type	Local 
#		At	WepR
#		Part	:PlantSeeds_Dust.part
#	End
#
#	Event
#
#		Type	Local 
#		At	WepL
#		Part	:PlantSeeds_Dust.part
#	End

End
			

End		