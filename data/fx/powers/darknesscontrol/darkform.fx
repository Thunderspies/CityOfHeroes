#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	Root
End

Input  
	InpName	Head
End

Input  
	InpName	hips
End

##################################
Condition
	On	Time
	Time	0
		

	Event	
		Ename   targetchill
		Type	Local
		At	Root
		
		#PMagnet	Head
		PMagnet		hips

		Part1	:DarkCloud.part
		Part2	:DarkCloud2.part
		Part3	:DarkCloudEmber.Part
		Part4	:DarkEmberFlat.Part
	End

		
End

Condition
	On	Time
	Time	60
		

	Event	
		Ename   targetchill
		Type	Local
		At	Root
		
		PMagnet		head
		
		Part1	:DarkCloudUp.part
		Part2	:DarkEmber.part		
	End

	Event	
		Ename   targetchill2
		Type	Local
		At	Root
		
		#PMagnet	Head
		PMagnet		Root
		
		Part2	:DarkCloudFlat.part
		
	End

End


#Condition
	#On	Time
	#Time	30
		

	#Event	
	#Ename   targetchill
	#	Type	Local
	#	At	Root
		
		#PMagnet	Head
		#PMagnet		Root

		#Part1	:CreateDarknessPuddleFlat.part
		#Part2	:CreateDarknessPuddle2Flat.part
		
	#End

#End


End				