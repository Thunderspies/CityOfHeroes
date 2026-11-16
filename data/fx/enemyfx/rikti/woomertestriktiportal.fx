#########################################################
##	RadiationAura
FxInfo


Input  
	InpName	root
End

##################################
	
Condition
	On	Time
	Time	0


	Event
		Ename	Hookup
		Type	start
		At	root
		Geom	RootToChestAdjustment
		
	End

	Event
		EName	Ring
		Type	local
		At	Hookup
		Part1	:RiktiPortalEllipse.part

		
	End

End

End				