#########################################################
##	template

FxInfo

LifeSpan	100
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



Condition
	On	Time
	Time	45

	Event
		ENAME	HeadGlow
		Type	Local
		At	Root
		part	CustomizeablePowers\Ninjitsu\WhiteSmoke.part
		part	CustomizeablePowers\Ninjitsu\BlackMist.part
		Lifespan 5

	End

End


Condition
	On	Time
	Time	46

	Event
		ENAME	HeadGlow
		Type	Local
		At	Root
		Part	CustomizeablePowers\Ninjitsu\SnapCap.part
		Part	CustomizeablePowers\Ninjitsu\SnapCapCore.part
		#Lifespan 5

	End

End


End