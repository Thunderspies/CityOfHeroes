#########################################################
##	Fireweapon
########################################################
FxInfo
LifeSpan	110  ##180 (I changed the time to match a shorter anim -steve)
######################################################################

Condition
	On 	Time
	Time 	15	


	Event
		
		Type	Local
		At	WepR
		BHVR	:WaterSwitch.bhvr
		Geom	WaterBackPackSwitch
		LifeSpan	80
		
		
	End

End


Condition
	On 	Time
	Time 	25

	Event
		
		Type	Local
		At	chest
		BHVR	:WaterPowerOffset.bhvr
		part	:WaterBackPackFountainMedium.part
		part	:WaterBackPackMistMedium.part
		part	:WaterBackPackbubbleMedium.part
		part	:WaterBackPack1bubbleMedium.part
		part	:WaterBackPackMistBright.part
		#pmagnet	T_Root
		#lookat	T_Root
		Sound bigextblast2 100 100 .33
		
	End

	

End

Condition
	On	Time
	Time	30

	Event
		Type	local
		At	Origin
		Sound bigextinqush_loop 100 100 .3
	End
End

End
##################################

