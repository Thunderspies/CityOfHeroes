#########################################################
##	FireRing
##
FxInfo
	
Input 
	InpName	Origin
End

Input
	InpName	Target
End

Input  
	InpName	Root
End

Input  
	InpName	Hips
End

#Lifespan 200

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start 
		At	Root
		Bhvr	behaviors/projectileTornado.bhvr
		Magnet	Target
		LookAt	Target
	End

	
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Ring
		Type	local 
		At	Prime
		Part1	:Ensnare.part
		Part2	:Ensnare2.part
		Part3	:EnsnareMist.part
		Part4	:Leaves.part
		Part5	:EnsnareMist2.part
		bhvr	behaviors/Wirlwind.bhvr	
		
	End

	Event
		EName 	Rings
		Type	Local 
		At	Prime
		Bhvr 	behaviors/tornado2.bhvr
		geom	FX_AirWisp01
		
	End

	
End


Condition
	On 	primehit

	Event
		EName 	Prime
		Type	Destroy 
	End

End



End			