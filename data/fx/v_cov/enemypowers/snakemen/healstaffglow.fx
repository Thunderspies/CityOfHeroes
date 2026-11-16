#########################################################
##	template

FxInfo

Lifespan 70
#########################################################
####################### AUDIO ###########################
#########################################################

#Condition
#	On	Time
#	Time	0
#	
#
##	Event
##		Type	Local
##		At 	Origin
##		Sound	Fast2 70 60 .7
##		Lifespan 80
##	End
#End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	23
	

	Event	
		Type	Local
		At	wepR
		
		BhvrOverride
			PositionOffset	0 0 2.3
		End

		part	:Healstaff.part
		part	:HealEnergystaff.part

		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 80
	
	End

#	Event	
#		ENAME	HeadGlow
#		Type	Local
#		At	Head
#		part	:HealEnergy.part
#		part	:Heal.part
#		#POther Chest
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 80
#		
#	End
	
End

End		