#########################################################
##	
##

FxInfo
#LifeSpan  72

Input  
	InpName	Target
End

Input  
	InpName	RingL
End

Input  
	InpName	WepR
End

##################################

## this would have been the actual flamethrower geometry, but it is now move to the enttype file


Condition
	On 	Time
	Time 	1

	Event
		EName 	base
		Type	start 
		At	root
#		geom	_basefunc_trtarc_baseA01__basetrtlodA
		geom	_basefunc_trtarc_baseB01__basetrtlodA
#		geom	_basefunc_trtarc_baseC01__basetrtlodA
		bhvroverride
#PyrRotate		0 -90 0
PositionOffset		6 0 0
		end
		
	End

######## small ##########

#	Event	
#		ename	Crystal
#		Type	start 
#		At	base
#		bhvroverride
#			PositionOffset		0 5.5 0
#		end
#		geom	_basefunc_trtarc_crystalA03_off__basetrtrotlodA ## 
#		
#	End

####### med ###########

	Event	
		ename	Crystal
		Type	start 
		At	base
		bhvroverride
			PositionOffset		0 5.5 0
		end
		geom	_basefunc_trtarc_crystalB03_off__basetrtrotlodA ##  
		
	End

####### lrg ###########

#	Event	
#		ename	Crystal
#		Type	start 
#		At	base
#		bhvroverride
#			PositionOffset		0 7.5 0
#		end
#		geom	_basefunc_trtarc_crystalC03_off__basetrtrotlodA ##  
#		
#	End
End


End