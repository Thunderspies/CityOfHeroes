#########################################################
##	Fireweapon
########################################################
FxInfo


Flags    InheritAlpha

########################################################

Condition	

#	Event
#		Type	Local 
#		At		ROOT
#		Geom	conetest 
#		bhvr	:weirdconeSpinOpposite.bhvr
#	 
#	End
#
#	Event
#		Type	Local 
#		At	ROOT
#		Geom	conetest 
#		bhvr	:weirdcone.bhvr
#	End

	Event
		Type	Local 
		At	ROOT
		ChildFx EnemyFX/Unique/Alarm/Alarm.fx
	End


End
##################################

