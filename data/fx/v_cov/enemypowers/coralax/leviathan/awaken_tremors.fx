#########################################################
##	pump_smoke
##
FxInfo

Input
	InpName Origin
End

Condition
	Event
		At	Origin
		Type	local
		Bhvr	:Awaken_Tremor.bhvr

		Lifespan 10
	End
End
#
#Condition
#	On	Cycle
#	Time	10
#
#	Event
#		At	Origin
#		Type	local
#		Bhvr	:Awaken_Tremor.bhvr
#		Lifespan 10
#	End
#End

End			