#########################################################
##	Fireweapon
########################################################
FxInfo

lifespan	120

Condition
	On 	Time
	Time 	26

#	Event
#		EName	Streak9
#		Type	local
#		At	HandL
#		Bhvr	Behaviors/PoisonSpikeScale.bhvr
#		Geom	PoisonSpikeLeft
#		
#		LifeSpan	33
#	End

	Event
		EName	Streak8
		Type	local
		At	HandR
		
		Bhvr	Behaviors/PoisonSpikeScale.bhvr
		Geom	PoisonSpikeRight
		
		LifeSpan	33
	End
End


Condition
	On 	Time
	Time 	83

	Event
		EName	streak
		Type	Destroy
		
	End
	

End


End
####################################################################
