#############################################################
## ShieldCharge_Attack.fx
#############################################################

FxInfo

Lifespan 100

Flags InheritAnimScale

#############################################################

Condition
	On time
	time 15

	event
		at root
		type local
		childfx :BurstOfSpeed_Child.fx
	End
End

#########################################################

End