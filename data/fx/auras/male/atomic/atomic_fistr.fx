#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	HandR
		Part	:Nucleus_Glow_Hand.part
	End
End

Condition
	On 	Cycle
	Time 	13

	Event
		Ename	ElectronWEPL
		Type	Local
		At	HandR
		ChildFX	:CHILD_Electron_Fists.fx
		Lifespan 40
	End
End

#############################################################

End