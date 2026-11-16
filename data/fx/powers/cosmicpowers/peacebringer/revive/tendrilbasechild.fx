#########################################################
##	FireAura
FxInfo
LifeSpan	200

################################################################
########################################################################################################

Condition
	On 	cycle
	Time 	2
	
	Event
		Ename	fireSound
		Type	Local	
		At	root
		
		Splat	Tendril01.tga
		Bhvr	:ReviveEnergyTendrils.bhvr

		lifeSpan	40
	End
End

#Condition
#	On 	cycle
#	Time 	2
#	
#	Event
#		Ename	fireSound
#		Type	Local	
#		At	root
#		
#		Splat	Tendril01.tga
#		Bhvr	:ReviveEnergyTendrils.bhvr
#
#		lifeSpan	160
#	End
#End
#
#Condition
#	On 	cycle
#	Time 	2
#	
#	Event
#		Ename	fireSound
#		Type	Local	
#		At	root
#		
#		Splat	Tendril01.tga
#		Bhvr	:ReviveEnergyTendrils.bhvr
#
#		lifeSpan	160
#	End
#End

End
