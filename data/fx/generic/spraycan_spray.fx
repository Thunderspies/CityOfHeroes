#########################################################
##	
##

FxInfo


Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

Input  
	InpName	Root
End

Input  
	InpName	head
End

Lifespan	80

##################################
Condition
	On 	Time
	Time 	17


	Event
		EName	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_spraycan
//		bhvr	behaviors/scale1a.bhvr	
	End

	Event
		EName	Tag
		Type	Local
		At	Root
		Geom	GEO_Graff_tag_01
	
	End

	Event
		EName	GasStream
		Type	Local
		At	Prime
		Altpiv   1
		
		Part1	:FireSpit.part
		Part2	:FireSpecksSpit.part
		Part3	:FireSpecksFalling.part
		Part4	:FireSpit2.part
		Part5	:FireSpitStart.part
		Sound arcoflame 100 100 .88
		
	End
	
End

End				