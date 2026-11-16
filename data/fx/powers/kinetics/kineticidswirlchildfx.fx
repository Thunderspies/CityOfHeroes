#########################################################
##	template

FxInfo

Input
	InpName Origin
End

Input  
	InpName	Target
End

Lifespan 300

######################################################




Condition
	On	Cycle
	Time	15

	Event	
		ENAME	HitSwirl1
		Type	Posit
		At	Origin
		part	:KineticIDCastSwirl01.part
		Lifespan 9
	End

End

End
