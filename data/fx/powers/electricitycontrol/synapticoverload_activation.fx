#############################################################
## JoltingChain_Activation.fx
#############################################################

FxInfo
LifeSpan	80

#############################################################

Input
	Inpname	Hips
End

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Glow.part
		Part3	:Hand_Ring.part
		Part	:Hand_Sparks.part
		POther	LArmR
	#	Sound	cagebolt2 80 30 .8
	End

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Body_Electricity_Arcs.part

		Part	:Hand_Glow.part
		Part	:Hand_Ring.part
		Part	:Hand_Sparks.part
	End
End

#############################################################

End