#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	Mystic
End

Input  
	InpName	Origin
End


##################################################################


##LifeSpan 900

Condition
	On	Time
	Time	0
	Event
		EName 	Prop
		Type	Start
		At	Root
		Geom	Prop_BoomBox_ground
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

End


Condition
On Time
Time 16
	Event
		Type	Local 
		At	Prop
		Sound Track1_loop 40.0 40.0 .7
		Sound Track2_loop 40.0 40.0 .9
		Sound Track3_loop 40.0 40.0 .9
		Sound Track4_loop 40.0 40.0 .9
		Sound Track5_loop 40.0 40.0 .9
		Sound Track6_loop 40.0 40.0 .9
		Sound Track7_loop 40.0 40.0 .9
		Sound Track8_loop 40.0 40.0 .9
		Sound Track9_loop 40.0 40.0 .9
		Sound Track10_loop 40.0 40.0 .9
		Sound Track11_loop 40.0 40.0 .9
		Sound Track12_loop 40.0 40.0 .9
		Sound Track13_loop 40.0 40.0 .9
		Sound Track14_loop 40.0 40.0 .9
		Sound Track15_loop 40.0 40.0 .9
		Sound Track16_loop 40.0 40.0 .9
		Sound Track17_loop 40.0 40.0 .7
		Sound Track18_loop 40.0 40.0 .7
		Sound Track19_loop 40.0 40.0 .8
		Sound Track20_loop 40.0 40.0 .8
		Sound Track21_loop 40.0 40.0 .9
		Sound Track22_loop 40.0 40.0 .8
		Sound Track23_loop 40.0 40.0 .7
		Sound Track24_loop 40.0 40.0 .8
		Sound Track25_loop 40.0 40.0 .8


	End

End




End


