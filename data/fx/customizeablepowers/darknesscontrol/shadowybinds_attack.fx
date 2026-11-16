#############################################################
## ShadowyBinds_Attack.fx
#############################################################

FxInfo

	Flags DontSuppress
lifespan 1

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DC_ShadowyBinds_Attack_01 100 100 .8
	End
End

#############################################################

End
