#########################################################
##	FireRing
##
FxInfo

LifeSpan	50	

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	root
End

Input  
	Inpname	head
End



##########################################################
Condition
	On 	Time
	Time 	0


	Event
		EName 	glow
		Type	Local 
		At	chest
		childFX	GENERIC/mu_med_bodyglow.fx
				
	End

End 


Condition

	On 	cycle
	Time 	1
		Chance	.5
	Event
		Type	Local
	
		At	root
		childfx	:mu_LightningBlastArc.fx
		Lifespan 10
		
	End

end

