#############################################################
## Fire_Impact_CHILD.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Ename	FlareUp
		Type	Local
		At	Root
		Part	:FireTrail_Alpha_out.part
		Part	:FireTrail_Alpha_Additive_out.part
		Part	:FireTrail_CoreAdd_out.part
	End
End

#########################################################

End